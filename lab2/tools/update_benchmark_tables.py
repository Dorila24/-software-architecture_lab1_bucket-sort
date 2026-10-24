"""Verify historical raw timings and refresh the README performance tables."""

import csv
from pathlib import Path
import statistics


ROOT = Path(__file__).resolve().parents[1]
BEGIN = "<!-- BEGIN VERIFIED BENCHMARK TABLES -->"
END = "<!-- END VERIFIED BENCHMARK TABLES -->"
LABELS = {"short": "Краткий", "detailed": "ТЗ по ГОСТ", "ordinary": "Обычный длинный"}
ROUTES = {"direct_E": "**E — baseline**", "Lab65_containment": "Eco.Lab65, включение",
          "Lab2_with_aggregate": "Eco.Lab2 с агрегатом"}


def read(name):
    with (ROOT / "deliverables" / name).open(newline="") as stream:
        return list(csv.DictReader(stream))


def verified_data():
    calls, raw_calls = read("benchmarks-calls.csv"), read("benchmarks-calls-raw.csv")
    sorting, raw_sort = read("benchmarks-sort.csv"), read("benchmarks-sort-raw.csv")
    assert len(calls) == len(sorting) == 36
    assert len(raw_calls) == len(raw_sort) == 540
    for row in calls:
        values = [float(r["ns_per_operation"]) for r in raw_calls
                  if r["run"].startswith(row["variant"] + "-")
                  and all(r[k] == row[k] for k in ("profile", "route", "operation"))]
        assert len(values) == 15
        for field, value in [("median_ns", statistics.median(values)),
                             ("min_ns", min(values)), ("max_ns", max(values))]:
            assert abs(float(row[field]) - value) < 1e-10, row
    for row in sorting:
        samples = [r for r in raw_sort if r["run"].startswith(row["variant"] + "-")
                   and all(r[k] == row[k] for k in ("profile", "distribution", "n"))]
        assert len(samples) == 15
        bucket = statistics.median(float(r["bucket_seconds"]) for r in samples) * 1000
        baseline = statistics.median(float(r["qsort_seconds"]) for r in samples) * 1000
        for field, value in [("bucket_ms", bucket), ("qsort_ms", baseline),
                             ("qsort_div_bucket", baseline / bucket)]:
            assert abs(float(row[field]) - value) < 1e-10, row
    return calls, sorting


def main():
    calls, sorting = verified_data()
    lines = [BEGIN, "## Скорость выполнения и baseline", "",
        "В PDF «Задание 2» (с. 1–4) нет отдельного требования измерять скорость или выбирать baseline. "
        "Требуются включение/агрегирование, переходы QueryInterface, исходники, сборка, UnitTest и Word-описание. "
        "Таблицы ниже дополняют эту проверку; baseline выбран в нашей методике.", "",
        "Это **сохранённые измерения из отчёта**, сверенные с 1 080 исходными строками замеров, "
        "а не значения времени из скринкаста. Время в строке PASS видео включает сборку и запуск клиента.", "",
        "### Сортировка: baseline — стандартная C `qsort`", "",
        "Тип `double`, 128 блоков, одинаковые копии входа, поэлементная проверка точного совпадения. "
        "Показаны два характерных размера из полного набора; [все 36 строк](deliverables/benchmarks-sort.csv) "
        "и [540 исходных замеров](deliverables/benchmarks-sort-raw.csv) доступны отдельно. "
        "Меньшее время лучше. Отношение — медиана времени qsort / медиана времени Bucket sort: "
        "значение > 1 означает преимущество Bucket sort, < 1 — преимущество qsort.", "",
        "| Промпт | Профиль | Вход и N | Bucket sort, мс | Baseline qsort, мс | qsort / Bucket |",
        "| --- | --- | --- | ---: | ---: | ---: |"]
    for row in sorting:
        if (row["distribution"], row["n"]) not in (("uniform", "50000"), ("cluster_reverse", "10000")):
            continue
        distribution = "Равномерный, 50 000" if row["distribution"] == "uniform" else "Обратный в одном блоке, 10 000"
        lines.append("| %s | %s | %s | %.3f | %.3f | %.3f |" % (
            LABELS[row["variant"]], row["profile"], distribution,
            float(row["bucket_ms"]), float(row["qsort_ms"]), float(row["qsort_div_bucket"])))
    lines += ["", "Во всех вариантах второй работы численный файл сортировки одинаков и взят из первой работы. "
        "Различия времени между строками не означают разные численные алгоритмы. "
        "При концентрации обратного массива в одном блоке сортировка вставками даёт квадратичный случай: "
        "на N=10 000 здесь Bucket sort примерно в 83–99 раз медленнее qsort.", "",
        "### Вызовы ACOM: baseline — прямое обращение к E", "",
        "`direct_E` — X/Y компонента E без внешних обёрток лабораторной. Сам E уже агрегирует B: "
        "это baseline выбранного маршрута, а не чистая арифметика C и не компонент без агрегирования. "
        "E предоставляет X и Y для всех четырёх операций; у B есть только X. "
        "Baseline измерялся заново в каждой ветке и профиле.", "",
        "| Промпт | Профиль | Маршрут | Арифметика, нс/вызов | QI(X) + Release, нс/пара |",
        "| --- | --- | --- | ---: | ---: |"]
    lookup = {(r["variant"], r["profile"], r["route"], r["operation"]): float(r["median_ns"]) for r in calls}
    for variant in LABELS:
        for profile in ("A", "C"):
            for route in ROUTES:
                a = lookup[(variant, profile, route, "arithmetic")]
                q = lookup[(variant, profile, route, "qi_release")]
                lines.append("| %s | %s | %s | %.3f | %.3f |" % (
                    LABELS[variant], profile, ROUTES[route], a, q))
    lines += ["", "Каждая ячейка — медиана 15 замеров (3 реализации × 5 повторов). "
        "Арифметический замер содержит 500 000 итераций по четыре операции, то есть 2 000 000 вызовов; "
        "вторая метрика — 500 000 пар QueryInterface(X) + Release. Проверяется контрольная сумма. "
        "Сборка — `-O2` без санитайзера; таймер — `clock()`, процессорное время процесса. "
        "Сервер MLX был остановлен перед историческими измерениями.", "",
        "Наносекунды получены делением времени большого цикла, а не измерением отдельного вызова. "
        "Маршруты используют разные реализации QI и зависимости, поэтому меньшее число у Lab2 "
        "не доказывает, что агрегирование само по себе ускоряет вызовы. Влияние прогрева, кэша, "
        "разрешения таймера и состояния одного Mac не исключено. Проверена macOS arm64; "
        "результаты не являются общей гарантией производительности.", "",
        "[Полный протокол](deliverables/sources-gost/provenance/benchmark-protocol.json) · "
        "[Сводка вызовов](deliverables/benchmarks-calls.csv) · "
        "[Исходные замеры вызовов](deliverables/benchmarks-calls-raw.csv)", END]
    path = ROOT / "README.md"
    text = path.read_text()
    block = "\n".join(lines)
    if BEGIN in text:
        start, stop = text.index(BEGIN), text.index(END) + len(END)
        text = text[:start] + block + text[stop:]
    else:
        text = text.replace("## Проверки и ограничения", block + "\n\n## Проверки и ограничения", 1)
    path.write_text(text)
    print("Verified 72 summary rows against 1080 samples; updated README tables.")


if __name__ == "__main__":
    main()

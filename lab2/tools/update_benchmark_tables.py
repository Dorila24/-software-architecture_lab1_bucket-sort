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
    names = {'short':'Краткий', 'detailed':'ТЗ по ГОСТ', 'ordinary':'Обычный подробный'}
    routes = {'direct_E':'E напрямую — сравнение', 'Lab65_containment':'Через Lab65',
              'Lab2_with_aggregate':'Через Lab2'}
    lines = [BEGIN, '<a id="скорость-выполнения-и-baseline"></a>',
        '## Скорость выполнения', '',
        'Для сортировки результат сопоставлялся со стандартной C `qsort`. Для арифметики и запросов интерфейса — '
        'с прямым вызовом компонента E. В задании № 2 отдельный образец сравнения скорости не предписан; это дополнение нашего исследования.', '',
        'Ниже сохранённые измерения из отчёта. Все 72 сводные строки сверены с 1 080 отдельными замерами. '
        'Время сборки из видео в эти таблицы не входит.', '',
        '### Сортировка', '',
        'Тип `double`, 128 блоков. Две сортировки получали одинаковые копии массива; результат сравнивался поэлементно. '
        'В таблице два характерных размера. Полный набор — [36 строк](deliverables/benchmarks-sort.csv) '
        'и [540 отдельных замеров](deliverables/benchmarks-sort-raw.csv). Чем меньше миллисекунд, тем быстрее.', '',
        '| Запрос | Сборка | Вход и длина | Блочная сортировка, мс | qsort, мс |',
        '| --- | --- | --- | ---: | ---: |']
    for r in sorting:
        if (r['distribution'], r['n']) not in (('uniform','50000'),('cluster_reverse','10000')):
            continue
        data = 'Равномерный, 50 000' if r['distribution']=='uniform' else 'Обратный в одном блоке, 10 000'
        lines.append('| %s | %s | %s | %.3f | %.3f |' % (names[r['variant']],r['profile'],data,float(r['bucket_ms']),float(r['qsort_ms'])))
    lines += ['', 'На равномерном массиве из 50 000 элементов блочная сортировка здесь быстрее qsort примерно в **1,66–1,75 раза**. '
        'На обратном массиве в одном блоке она медленнее примерно в **83–99 раз**: сортировка вставками внутри большого блока требует много перемещений.', '',
        'Численный файл во всех вариантах один и тот же, из первой работы. Различия между строками не означают, '
        'что разные запросы создали разные алгоритмы сортировки.', '',
        '### Арифметика и переходы между интерфейсами', '',
        'В строках «E напрямую» показан образец для сравнения. E предоставляет X и Y для всех четырёх операций и сам агрегирует B. '
        'Это конкретный маршрут без внешних обёрток лабораторной; он не равен простой арифметике C. У одного B есть только интерфейс X.', '',
        '| Запрос | Сборка | Как выполнялся вызов | Арифметика, нс/вызов | Запрос X и Release, нс/пара |',
        '| --- | --- | --- | ---: | ---: |']
    lookup={(r['variant'],r['profile'],r['route'],r['operation']):float(r['median_ns']) for r in calls}
    for variant in names:
        for profile in ('A','C'):
            for route in routes:
                lines.append('| %s | %s | %s | %.3f | %.3f |' % (names[variant],profile,routes[route],
                    lookup[(variant,profile,route,'arithmetic')],lookup[(variant,profile,route,'qi_release')]))
    lines += ['', 'Каждая ячейка — медиана 15 замеров: три реализации по пять повторов. '
        'Для арифметики выполнялись 500 000 итераций по четыре операции, то есть 2 000 000 вызовов. '
        'Для перехода измерялись 500 000 пар `QueryInterface(X)` и `Release`. Контрольная сумма арифметики проверялась.', '',
        'Замеры сделаны с `-O2` без UBSan, после остановки сервера MLX. Таймер `clock()` учитывает процессорное время процесса. '
        'Наносекунды получены делением времени большого цикла, а не прямым измерением отдельного вызова.', '',
        'Меньшее число у Lab2 не доказывает, что агрегирование само по себе ускоряет программу: маршруты отличаются реализациями '
        'методов и зависимостями. Прогрев, кэш, разрешение таймера и состояние ноутбука также влияют на результат. '
        'Эти значения относятся к проверенному Mac, а не ко всем компьютерам.', '',
        '[Методика измерений](deliverables/sources-gost/provenance/benchmark-protocol.json) · '
        '[Полная таблица вызовов](deliverables/benchmarks-calls.csv) · '
        '[Отдельные замеры вызовов](deliverables/benchmarks-calls-raw.csv)', END]
    path=ROOT/'README.md'
    text=path.read_text()
    block='\n'.join(lines)
    if BEGIN in text:
        a,b=text.index(BEGIN),text.index(END)+len(END)
        text=text[:a]+block+text[b:]
    else:
        target='## Исходники и файлы для проверки'
        assert target in text
        text=text.replace(target,block+'\n\n'+target,1)
    path.write_text(text)
    print('Verified 72 summary rows against 1080 samples; updated Russian tables.')


if __name__ == '__main__':
    main()

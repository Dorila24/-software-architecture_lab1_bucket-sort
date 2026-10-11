"""Verify published sources, measurement ledgers and report without an SDK."""
from pathlib import Path
import hashlib
import json
from zipfile import ZipFile

root = Path(__file__).resolve().parents[1]
manifest = json.loads((root / "manifest-sha256.json").read_text())
for name, expected in manifest.items():
    path = root / name
    assert path.is_file(), "Missing file: " + name
    assert path.stat().st_size == expected["bytes"], "Size changed: " + name
    assert hashlib.sha256(path.read_bytes()).hexdigest() == expected["sha256"], "Hash changed: " + name
experiment = next((root / "runs").glob("experiment-*"))
entries = json.loads((experiment / "experiment.json").read_text())["runs"]
assert len(entries) == 6
requests = 0
for entry in entries:
    run = experiment / entry["path"]
    record = json.loads((run / "run.json").read_text())
    ledger = [json.loads(line) for line in (run / "requests.jsonl").read_text().splitlines()]
    assert record["status"] == "passed"
    assert len(ledger) == record["metrics"]["requests"]
    assert sum(row["usage"]["prompt_tokens"] for row in ledger) == record["metrics"]["prompt_tokens_known"]
    assert sum(row["usage"]["completion_tokens"] for row in ledger) == record["metrics"]["completion_tokens_known"]
    assert hashlib.sha256((root / record["source"]).read_bytes()).hexdigest() == record["source_sha256"]
    assert len(list(run.glob("followup-*.txt"))) == record["user_followups"]
    requests += len(ledger)
assert requests == 100
with ZipFile(root / "reports/report-comparison.docx") as archive:
    assert archive.testzip() is None
    text = archive.read("word/document.xml").decode()
    assert "Фирсова Валерия Дмитриевна" in text and "24ТИДИ" in text
    assert "[ФИО]" not in text and "[группа]" not in text
print("Verified publication hashes, six original sources, 100 request ledgers and personalized Word report.")

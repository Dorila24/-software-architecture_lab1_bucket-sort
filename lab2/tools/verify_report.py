"""Check that the edited Word report still agrees with the original measurements."""
import argparse
import csv
import hashlib
import json
from pathlib import Path
import statistics
from xml.etree import ElementTree as ET
from zipfile import ZipFile

ROOT = Path(__file__).resolve().parents[1]
NS = {'w':'http://schemas.openxmlformats.org/wordprocessingml/2006/main',
      'm':'http://schemas.openxmlformats.org/officeDocument/2006/math'}
LABELS={'short':'Краткий','detailed':'ТЗ по ГОСТ','ordinary':'Подробный без ТЗ'}


def ledger(name):
    with (ROOT/'deliverables'/name).open(newline='') as f:
        return list(csv.DictReader(f))


def value(text):
    return float(text.replace('≥','').replace(' ','').replace('\xa0','').replace(',','.'))


def xml_document(path):
    with ZipFile(path) as z:
        assert z.testzip() is None
        return ET.fromstring(z.read('word/document.xml'))


def text(node):
    return ''.join(x.text or '' for x in node.findall('.//w:t',NS))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--audit-json',type=Path)
    args=parser.parse_args()
    report=ROOT/'deliverables/report-comparison.docx'
    document=xml_document(report)
    paragraphs=[text(p) for p in document.findall('./w:body/w:p',NS)]
    body=' '.join(' '.join(paragraphs).split())
    assert 'Фирсова Валерия Дмитриевна' in body and '24ТИДИ' in body
    assert '[ФИО]' not in body and '[группа]' not in body
    assert len(document.findall('.//m:oMath',NS))==2
    for name in ['short.txt','detailed.txt']:
        assert ' '.join((ROOT/'experiment'/name).read_text().split()) in body,name
    tables=[]
    for table in document.findall('./w:body/w:tbl',NS):
        tables.append([[text(c) for c in r.findall('./w:tc',NS)] for r in table.findall('./w:tr',NS)])
    assert len(tables)==12
    runs=ledger('generation-measurements.csv')
    details=tables[10][1:]
    assert len(details)==9
    for r in runs:
        matches=[x for x in details if x[0]==LABELS[r['variant']] and x[1]==r['repeat']]
        assert len(matches)==1
        x=matches[0]
        for i,key in [(2,'input_tokens'),(3,'output_tokens'),(4,'http_requests'),(5,'manual_followups')]:
            assert value(x[i])==int(r[key]),(r['run'],key)
        assert abs(value(x[6])-float(r['active_seconds'])/60)<=.00501
        assert x[2].startswith('≥')==(r['tokens_complete']=='False')
        assert x[3].startswith('≥')==(r['tokens_complete']=='False')
        assert x[4].startswith('≥')==(r['requests_complete']=='False')
        assert x[6].startswith('≥')==(r['timing_complete']=='False')
    for label,name in LABELS.items():
        subset=[r for r in runs if r['variant']==label]
        row=next(r for r in tables[5][1:] if r[0]==name)
        assert row[1]=='3 из 3'
        assert value(row[2])==statistics.median(int(r['total_tokens']) for r in subset)
        assert value(row[3])==statistics.median(int(r['http_requests']) for r in subset)
        assert abs(value(row[4])-statistics.median(float(r['active_seconds']) for r in subset)/60)<=.00501
    first=ledger('generation-initial-phase.csv')
    for label,name in LABELS.items():
        subset=[r for r in first if r['run'].startswith(label+'-')]
        row=next(r for r in tables[4][1:] if r[0]==name)
        assert row[1]=='0 из 3'
        assert value(row[2])==statistics.median(int(r['input_tokens']) for r in subset)
        assert value(row[3])==statistics.median(int(r['output_tokens']) for r in subset)
        assert abs(value(row[4])-statistics.median(float(r['active_seconds']) for r in subset))<=.00501
    sorting=ledger('benchmarks-sort.csv')
    assert len(tables[7][1:])==12
    for row in tables[7][1:]:
        label=next(k for k,v in LABELS.items() if v==row[0])
        dist='uniform' if row[2].startswith('Равномерный') else 'cluster_reverse'
        n='50000' if dist=='uniform' else '10000'
        r=next(x for x in sorting if (x['variant'],x['profile'],x['distribution'],x['n'])==(label,row[1],dist,n))
        assert abs(value(row[3])-float(r['bucket_ms']))<=.000501
        assert abs(value(row[4])-float(r['qsort_ms']))<=.000501
    calls=ledger('benchmarks-calls.csv')
    routes={'E напрямую':'direct_E','Через Lab65':'Lab65_containment','Через Lab2':'Lab2_with_aggregate'}
    assert len(tables[8][1:])==18
    for row in tables[8][1:]:
        label=next(k for k,v in LABELS.items() if v==row[0])
        for i,op in [(3,'arithmetic'),(4,'qi_release')]:
            r=next(x for x in calls if (x['variant'],x['profile'],x['route'],x['operation'])==(label,row[1],routes[row[2]],op))
            assert abs(value(row[i])-float(r['median_ns']))<=.000501
    old=xml_document(ROOT/'docs/editorial-history/20261025/report-before-rewrite.docx')
    previous=[]
    for t in old.findall('./w:body/w:tbl',NS):
        previous.append([[text(c) for c in r.findall('./w:tc',NS)] for r in t.findall('./w:tr',NS)])
    for i in [1,2,3,4,5,6,8,9]:
        assert tables[9][i][1]==previous[6][i][1],('model settings',i)
    assert tables[11][1:]==previous[8][1:]
    result={'passed':True,'report_sha256':hashlib.sha256(report.read_bytes()).hexdigest(),
            'author_and_group_verified':True,'original_prompts_preserved':['short.txt','detailed.txt'],
            'generation_runs_reconciled':9,'initial_groups_reconciled':3,'full_cost_groups_reconciled':3,
            'sort_summary_rows_reconciled':12,'call_summary_rows_reconciled':18,
            'parameters_and_full_costs_preserved':True,'native_word_equations':2,
            'note':'Checks current document data. Rendering and visual review are recorded separately; historical component tests were not rerun for this text edit.'}
    if args.audit_json:args.audit_json.write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n')
    print('REPORT_VERIFIED: 9 runs, 3 first-answer groups, 3 final groups, 12 sorting rows, 18 call rows, parameters and exact prompts.')


if __name__=='__main__':
    main()

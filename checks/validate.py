"""Frozen independent validator; no model-owned tests are trusted."""
from pathlib import Path
import subprocess
import sys
import re

workspace,builder,client=sys.argv[1:4]
source=Path(workspace)/"SourceFiles/BucketMethods.c"
text=source.read_text()
# Code screening supplements compiler symbol checks and manual review.
clean=re.sub(r"/\*.*?\*/", "", text, flags=re.S)
if re.search(r"#\s*include\s*<",clean):
    raise SystemExit("Component may include only the supplied headers, not standard library headers")
if re.search(r"\b(?:malloc|calloc|realloc|free|memcpy|memmove|memset|qsort|bsearch|floor|ceil|fabs|pow|sqrt)\s*\(",clean):
    raise SystemExit("Forbidden library call in component")
command=[sys.executable,builder,"--project",workspace,"--client",client,"--run","--sanitize","--output","BuildFiles/Independent"]
result=subprocess.run(command,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
print(result.stdout)
raise SystemExit(result.returncode)

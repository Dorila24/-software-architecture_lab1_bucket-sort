"""Frozen, standalone build driver for the lab 2 composition experiment."""
from pathlib import Path
import argparse,hashlib,json,subprocess,sys,time

def call(argv,cwd):
    r=subprocess.run(list(map(str,argv)),cwd=cwd,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if r.returncode:
        print(r.stdout)
        raise RuntimeError("Command failed: "+str(argv[0]))
    return r.stdout

def build(project,client=None,profile="A",sanitize=False,compile_only=False,run=False,output="BuildFiles/Reuse"):
    project=Path(project).resolve();sdk=project/".sdk";dest=project/output
    if not dest.resolve().is_relative_to(project):raise ValueError("Output must be inside workspace")
    dest.mkdir(parents=True,exist_ok=True)
    tools=json.loads((project/".arpo-toolchain.json").read_text())
    flags=["-std=c89","-pedantic-errors","-Werror=implicit-function-declaration","-DECO_LIB","-DUGUID_UTILITY","-DECO_APPLE","-DECO_MAC","-DECO_AARCH64","-DERR_ECO_SUCCESES=ERR_ECO_SUCCESS","-O2","-isysroot",tools["sdkroot"]]
    for name in ["SharedFiles","HeaderFiles"]:flags += ["-I",str(project/name)]
    for h in sorted(sdk.glob("*/SharedFiles")): flags += ["-isystem",str(h)]
    for h in sorted((project/"Vendor").glob("*/SharedFiles")): flags += ["-isystem",str(h)]
    for h in sorted((project/"Vendor").glob("*/HeaderFiles")): flags += ["-isystem",str(h)]
    if sanitize:flags += ["-fsanitize=undefined","-fno-sanitize-recover=undefined"]
    objects=[];symbols={};sources=sorted((project/"SourceFiles").glob("*.c"))+sorted((project/"Vendor").glob("*/SourceFiles/*.c"))
    for i,source in enumerate(sources):
        extra=["-Wno-comment","-Wno-newline-eof"] if "Vendor" in source.relative_to(project).parts else []
        if "Eco.CalculatorC" in source.parts:
            extra += ["-DGetIEcoComponentFactoryPtr_4828F6552E4540E78121EBD220DC360E=teacher_calculator_c_factory","-Dg_x4828F6552E4540E78121EBD220DC360EFactoryVTbl=teacher_calculator_c_factory_vtable","-Dg_x4828F6552E4540E78121EBD220DC360EFactory=teacher_calculator_c_factory_object"]
        obj=dest/("component-%03d.o"%i)
        call([tools["clang"],*flags,*extra,"-c",source,"-o",obj],project)
        undefined=call([tools["nm"],"-u",obj],project)
        symbols[source.relative_to(project).as_posix()]=undefined.split()
        if "Vendor" not in source.relative_to(project).parts:
            forbidden={"_malloc","_calloc","_realloc","_free","_memcpy","_memmove","_memset","_bzero","_qsort","_printf","_fprintf","_puts"}
            if forbidden.intersection(undefined.split()):raise ValueError("Forbidden libc dependency in "+source.name+": "+undefined)
        objects.append(obj)
    (dest/"symbols.json").write_text(json.dumps(symbols,indent=2)+"\n")
    archive=dest/"liblab2.a";archive.unlink(missing_ok=True)
    call([tools["ar"],"rcs",archive,*objects],project)
    if compile_only:
        print("REUSE_COMPILE_OK sources="+str(len(sources)))
        return
    client=Path(client).resolve() if client else project/"UnitTestFiles/SourceFiles/EcoLab2.c"
    libraries=[]
    for name in ["Eco.System1","Eco.InterfaceBus1","Eco.MemoryManager1","Eco.FileSystemManagement1"]:
        matches=list((sdk/name/"BuildFiles/Mac/arm64/StaticRelease").glob("lib*.a"))
        if len(matches)!=1:raise ValueError("Missing SDK library: "+name)
        libraries.extend(matches)
    executable=dest/"client"
    call([tools["clang"],*flags,"-DTEACHER_PROFILE_"+profile,client,archive,*libraries,"-o",executable],project)
    record={"profile":profile,"sanitize":sanitize,"sources":[str(s.relative_to(project)) for s in sources],"symbols":symbols}
    if run:
        record["output"]=call([executable],project)
        print(record["output"],end="")
    (dest/"build-result.json").write_text(json.dumps(record,indent=2)+"\n")
    print("REUSE_BUILD_OK profile="+profile)

def main():
    a=argparse.ArgumentParser();a.add_argument("--project",default=".");a.add_argument("--client");a.add_argument("--profile",choices=["A","C"],default="A");a.add_argument("--sanitize",action="store_true");a.add_argument("--compile-only",action="store_true");a.add_argument("--run",action="store_true");a.add_argument("--output",default="BuildFiles/Reuse")
    try:build(**vars(a.parse_args()))
    except (RuntimeError,ValueError,OSError) as e:print("BUILD_ERROR",e);return 1
    return 0
if __name__=="__main__":sys.exit(main())

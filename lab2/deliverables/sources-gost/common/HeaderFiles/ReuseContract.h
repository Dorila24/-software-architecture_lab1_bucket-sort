/* Frozen behavioral contract for the lab 2 experiment. No implementation supplied.
 * Reuse the exact five sorting methods in BucketMethods.c; do not edit that file.
 * All object state, creation, QI, references and composition belong in Reuse.c.
 * Existing ArpoMethods.h comments about supplied lifetime apply to lab 1 only.
 *
 * createEcoLab65 and createEcoLab2 are called by supplied static factories.
 * Both clear *out before checking other arguments. Null out/system/iid fail.
 * Lab65: when outer!=0, creation is allowed ONLY with IID_IEcoUnknown;
 * return the private nondelegating unknown, locally owned with one reference.
 * Public Lab65, X and Y must use controlling-unknown QI/AddRef/Release.
 * Standalone their canonical unknown is the object's own nondelegating unknown.
 * Lab2 rejects outer!=0. It supports Lab2, Lab65, X, Y and Unknown.
 * GetVersion returns 2. Every successful QI owns one matching reference.
 * Unsupported IID fails, clears output, and does not change references.
 * All interfaces of one public object have equal canonical Unknown identities;
 * supported sets are static and QI must be reflexive, symmetric, transitive.
 *
 * Lab65 contains an X instance of A (or C under the same teacher CID), and
 * a Y instance of D. Its own X/Y wrappers forward the actual arithmetic calls.
 * Lab2 contains Y of E and aggregates Lab65 with its controlling Unknown.
 * Lab2 directly exposes the aggregated Lab65 interface. Its own X and Y wrappers
 * forward to the contained E (whose X is implemented by aggregated B). Do not replace the calculators with locally computed arithmetic.
 * A and C intentionally have the same CID in the teaching repository. Independent
 * A and C registry profiles select the actual original factory before creation.
 * D internally contains B; E internally aggregates B. B must remain registered.
 * Acquire every inner object through the real QueryComponent, not direct alloc.
 * Hold private nondelegating unknowns for aggregates, not permanent public refs
 * that delegate to the outer and form a cycle. Do not AddRef the stored outer.
 * Free every acquired dependency/allocator/system reference on destruction and
 * on failed construction. Free object storage before releasing its allocator.
 * Preserve external interface references after releasing the initial interface.
 * Missing system/bus/allocator/inner dependencies or allocation failure must fail
 * construction without publishing a partial object, leaks or dangling pointers.
 * Use actual SDK errors and allocator; no libc, math.h or third-party calls.
 * Calculator operands in independent tests are [-100,100], divisor nonzero;
 * all tested differences and integer quotients fit int16_t, products fit int32_t.
 * Sorting input/status behavior is the unchanged contract of laboratory 1.
 */
#ifndef REUSE_CONTRACT_H
#define REUSE_CONTRACT_H
#include "IEcoSystem1.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IdEcoMemoryManager1.h"
#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"
#include "IEcoLab65.h"
#include "IEcoLab2.h"
#include "ArpoMethods.h"
int16_t ECOCALLMETHOD createEcoLab65(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out);
int16_t ECOCALLMETHOD createEcoLab2(IEcoUnknown* system, IEcoUnknown* outer, const UGUID* iid, void** out);
#endif

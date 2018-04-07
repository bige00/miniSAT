/****************************************************************************************[XAlloc.h]
Copyright (c) 2009-2010, Niklas Sorensson

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/


#ifndef Minisat_XAlloc_h
#define Minisat_XAlloc_h

#include <errno.h>
#include <stdlib.h>

namespace Minisat {

//=================================================================================================
// Simple layer on top of malloc/realloc to catch out-of-memory situtaions and provide some typing:

class OutOfMemoryException{};
static inline void* xrealloc(void *ptr, size_t size)
{
    void* mem = realloc(ptr, size);
    if (mem == NULL && errno == ENOMEM){
        throw OutOfMemoryException();
    }else
        return mem;
}

//=================================================================================================
}

#endif

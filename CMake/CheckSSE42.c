/* Check for SSE 4.2.  SSE 4.2 was first supported in Nehalem processors
   introduced in November, 2008.  This does not check for the existence of the
   cpuid instruction itself, which was introduced on the 486SL in 1992, so this
   will fail on earlier x86 processors.  cpuid works on all Pentium and later
   processors. */
#define CHECK_SSE42(have)                                     \
  do {                                                        \
    unsigned int eax, ecx;                                    \
    eax = 1;                                                  \
    __asm__("cpuid" : "=c"(ecx) : "a"(eax) : "%ebx", "%edx"); \
    (have) = (ecx >> 20) & 1;                                 \
} while (0)

int main(void)
{
    int sse42;

    CHECK_SSE42(sse42);
    return sse42;
}


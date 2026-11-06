
#ifndef MacrosH
#define MacrosH

#define    _CONCAT(A, B)    A##B
#define     CONCAT(A, B)    _CONCAT(A, B)
#define     ARRAYSIZE(X)    (sizeof(X) / sizeof(X[0]))

#endif

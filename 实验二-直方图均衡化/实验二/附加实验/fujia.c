#include <stdio.h>
#include <memory.h>
#include <math.h>
#define DEBUG
const int N = 8;
int main()
{
    int i, t;
    double r[N];
    double n1[] = { 790,1023,850,656,329,245,122,81 };
    double pr[N];
    double sk1[N];
    double sk2[N];
    double n2[N];
    double n3[N];
    double ps[N];

    for (i = 0; i < N; ++i)
        pr[i] = n1[i] / 4096.0;

    memcpy(sk1, pr, sizeof(sk1));
    for (i = 1; i < N; ++i)
        sk1[i] += sk1[i - 1];

    for (i = 0; i < N; ++i)
        sk2[i] = floor(sk1[i] * (N - 1) + 0.5);

    memset(n2, 0, sizeof(n2));
    for (i = 0; i < N; ++i)
        n2[(int)(sk2[i])] += n1[i];

    i = 0; t = 0;
    for (i = 0; i < N; ++i)
        if (n2[i] != 0)
            n3[t++] = n2[i];
    for (i = 0; i < t; ++i)
        ps[i] = n3[i] / 4096;

#ifdef DEBUG
    for (i = 0; i < N; ++i)
        printf("%f ", sk1[i]);
    printf("\n");
    for (i = 0; i < N; ++i)
        printf("%f ", pr[i]);
    printf("\n");
    for (i = 0; i < N; ++i)
        printf("%f ", sk2[i]*7);
    printf("\n");
    for (i = 0; i < N; ++i)
        printf("%f ", n2[i]);
    printf("\n");
    for (i = 0; i < t; ++i)
        printf("%f ", n3[i]);
    printf("\n");
    for (i = 0; i < t; ++i)
        printf("%f ", ps[i]);
    printf("\n");
#endif

    return 0;

}

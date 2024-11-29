#include <stdio.h>
#include <math.h>

struct Complex{
    double re;
    double im;
};

struct Complex ComplexConjugate(struct Complex complex){
    struct Complex conj = {complex.re, -complex.im};
    return conj;
}

struct Complex Addition(struct Complex complexA, struct Complex complexB){
    struct Complex sum = {complexA.re + complexB.re, complexA.im + complexB.im};
    return sum;
}

struct Complex subtraction(struct Complex complexA, struct Complex complexB){
    struct Complex diff = {complexA.re - complexB.re, complexA.im - complexB.im};
    return diff;
}

struct Complex multiplication(struct Complex complexA, struct Complex complexB){
    struct Complex product = {complexA.re * complexB.re - complexA.im * complexB.im, complexA.re * complexB.im + complexA.im * complexB.re};
    return product;
}

struct Complex division(struct Complex complexA, struct Complex complexB){
    struct Complex quotient = {(complexA.re * complexB.re + complexA.im * complexB.im) / (complexB.re * complexB.re + complexB.im * complexB.im), (complexA.im * complexB.re - complexA.re * complexB.im) / (complexB.re * complexB.re + complexB.im * complexB.im)};
    return quotient;
}

struct Complex reciprocal(struct Complex complex){
    struct Complex rec = {complex.re / (complex.re * complex.re + complex.im * complex.im), -complex.im / (complex.re * complex.re + complex.im * complex.im)};
    return rec;
}

double magnitude(struct Complex complex){
    return sqrt(complex.re * complex.re + complex.im * complex.im);
}

int main() {

    struct Complex complexA;
    printf("Enter the real part of the first complex number: ");
    scanf("%lf", &complexA.re);
    printf("Enter the imaginary part of the first complex number: ");
    scanf("%lf", &complexA.im);

    struct Complex complexB;
    printf("Enter the real part of the second complex number: ");
    scanf("%lf", &complexB.re);
    printf("Enter the imaginary part of the second complex number: ");
    scanf("%lf", &complexB.im);

    struct Complex sum = Addition(complexA, complexB);
    struct Complex diff = subtraction(complexA, complexB);
    struct Complex prod = multiplication(complexA, complexB);   
    struct Complex qout = division(complexA, complexB);   

    printf("sum: %f + %fi\n", sum.re, sum.im);
    printf("diff: %f + %fi\n", diff.re, diff.im);
    printf("prod: %f + %fi\n", prod.re, prod.im);
    printf("qout: %f + %fi\n", qout.re, qout.im);

    return 0;
}
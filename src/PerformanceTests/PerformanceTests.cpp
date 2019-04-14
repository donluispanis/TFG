#include "TestTemplate.h"
#include <cmath>

void voidF()
{
    return;
}

int main()
{
    TestTemplate T("bin/PerformanceResults.txt");

    T.WriteMessageIntoScreenAndFile("----------------------------------------------------\n|");
    T.WriteMessageIntoScreenAndFile("| Notice:  This is a 30 tests suite\n"
                                    "|          Each Test is averaged over a total of 100 tests repeated\n"
                                    "|          Inside every test, each operation is reapeated 10000000 times\n"
                                    "|          in order to be measurable in time\n|");

    T.ExecuteTest([]() {for (int j = 0; j < 10000000; j++); });
    T.WriteTestResultsIntoScreenAndFile("01", "Void For Loop with I++", T.CalculateAverageTime());

    T.ExecuteTest([]() {for (int j = 0; j < 10000000; ++j); });
    T.WriteTestResultsIntoScreenAndFile("02", "Void For Loop with ++I", T.CalculateAverageTime());

    int i;
    T.ExecuteTest([&i]() {
        for (int j = 0; j < 10000000; ++j) {
            i = j;
    } });
    T.WriteTestResultsIntoScreenAndFile("03", "Simple assignment (=)", T.CalculateAverageTime());
    
    {
        int *k = new int[100000000];
        T.CalculateAverageTime();
        T.ExecuteTest([&k]() {
        for(int j = 0; j < 10000; j++){
            for(int i = 0; i < 10000; i++)
            {
                k[j * 10000 + i] = 100;
            }
        } });
        T.WriteTestResultsIntoScreenAndFile("04", "Aligned memory access", T.CalculateAverageTime());
        delete[] k;

        k = new int[100000000];
        T.CalculateAverageTime();
        T.ExecuteTest([&k]() {
        for(int i = 0; i < 10000; i++){
            for(int j = 0; j < 10000; j++)
            {
                k[j * 10000 + i] = 100;
            }
        } });
        T.WriteTestResultsIntoScreenAndFile("05", "Unaligned memory access", T.CalculateAverageTime());
        delete[] k;
    }

    int *k = new int[10000000];
    T.ExecuteTest([k, &i]() {
        for (int j = 0; j < 10000000; ++j) {
            k[j] = j;
    } });
    T.WriteTestResultsIntoScreenAndFile("04", "Simple assignment (=) + Array access ([]) ((j in scope)k[j] = j)", T.CalculateAverageTime());

    T.ExecuteTest([k, &i]() {
        for (int j = 0; j < 10000000; ++j) {
            k[j] = i;
    } });
    T.WriteTestResultsIntoScreenAndFile("05", "Simple assignment (=) + Array access ([]) ((j in scope, i from out) k[j] = i)", T.CalculateAverageTime());

    T.ExecuteTest([k, &i]() {
        for (int j = 0; j < 10000000; ++j) {
            k[i] = j;
    } });
    T.WriteTestResultsIntoScreenAndFile("06", "Simple assignment (=) + Array access ([]) ((j in scope, i from out) k[i] = j;)", T.CalculateAverageTime());

    T.WriteMessageIntoScreenAndFile("|---------------------------------------------------\n|");

    T.ExecuteTest([k, &i]() {
        for (int j = 0; j < 10000000; ++j) {
            k[j] += j;
    } });
    T.WriteTestResultsIntoScreenAndFile("07", "(int) Sum", T.CalculateAverageTime());

    T.ExecuteTest([k, &i]() {
        for (int j = 0; j < 10000000; ++j) {
            k[j] -= j;
    } });
    T.WriteTestResultsIntoScreenAndFile("08", "(int) Subtraction", T.CalculateAverageTime());

    T.ExecuteTest([k, &i]() {
        for (int j = 0; j < 10000000; ++j) {
            k[j] *= j;
    } });
    T.WriteTestResultsIntoScreenAndFile("09", "(int) Multiplication", T.CalculateAverageTime());

    T.ExecuteTest([k, &i]() {
        for (int j = 0; j < 10000000; ++j) {
            k[j] /= i;
    } });
    T.WriteTestResultsIntoScreenAndFile("10", "(int) Division", T.CalculateAverageTime());

    T.ExecuteTest([k, &i]() {
        for (int j = 0; j < 10000000; ++j) {
            k[j] %= i;
    } });
    T.WriteTestResultsIntoScreenAndFile("11", "(int) Modulus", T.CalculateAverageTime());

    T.WriteMessageIntoScreenAndFile("|---------------------------------------------------\n|");

    long int li = rand();
    long int *lk = new long int[10000000];

    for (int j = 0; j < 999999; j++)
        lk[j] = rand();

    T.ExecuteTest([lk, &li]() {
        for (long int j = 0; j < 10000000; ++j) {
            lk[j] += li;
    } });
    T.WriteTestResultsIntoScreenAndFile("12", "(long int) Sum", T.CalculateAverageTime());

    T.ExecuteTest([lk, &li]() {
        for (long int j = 0; j < 10000000; ++j) {
            lk[j] -= li;
    } });
    T.WriteTestResultsIntoScreenAndFile("13", "(long int) Subtraction", T.CalculateAverageTime());

    T.ExecuteTest([lk, &li]() {
        for (long int j = 0; j < 10000000; ++j) {
            lk[j] *= li;
    } });
    T.WriteTestResultsIntoScreenAndFile("14", "(long int) Multiplication", T.CalculateAverageTime());

    T.ExecuteTest([lk, &li]() {
        for (long int j = 0; j < 10000000; ++j) {
            lk[j] /= li;
    } });
    T.WriteTestResultsIntoScreenAndFile("15", "(long int) Division", T.CalculateAverageTime());

    T.ExecuteTest([lk, &li]() {
        for (long int j = 0; j < 10000000; ++j) {
            lk[j] %= li;
    } });
    T.WriteTestResultsIntoScreenAndFile("16", "(long int) Modulus", T.CalculateAverageTime());

    T.WriteMessageIntoScreenAndFile("|---------------------------------------------------\n|");

    float fi = rand() * 0.555;
    float *fk = new float[10000000];

    for (int j = 0; j < 999999; j++)
        fk[j] = rand() * 0.555;

    T.ExecuteTest([fk, &fi]() {
        for (int j = 0; j < 10000000; ++j) {
            fk[j] += fi;
    } });
    T.WriteTestResultsIntoScreenAndFile("17", "(float) Sum", T.CalculateAverageTime());

    T.ExecuteTest([fk, &fi]() {
        for (int j = 0; j < 10000000; ++j) {
            fk[j] -= fi;
    } });
    T.WriteTestResultsIntoScreenAndFile("18", "(float) Subtraction", T.CalculateAverageTime());

    T.ExecuteTest([fk, &fi]() {
        for (int j = 0; j < 10000000; ++j) {
            fk[j] *= fi;
    } });
    T.WriteTestResultsIntoScreenAndFile("19", "(float) Multiplication", T.CalculateAverageTime());

    T.ExecuteTest([fk, &fi]() {
        for (int j = 0; j < 10000000; ++j) {
            fk[j] /= fi;
    } });
    T.WriteTestResultsIntoScreenAndFile("20", "(float) Division", T.CalculateAverageTime());

    T.WriteMessageIntoScreenAndFile("|---------------------------------------------------\n|");

    double di = rand() * 0.555;
    double *dk = new double[10000000];

    for (int j = 0; j < 999999; j++)
        dk[j] = rand() * 0.555;

    T.ExecuteTest([dk, &di]() {
        for (int j = 0; j < 10000000; ++j) {
            dk[j] += di;
    } });
    T.WriteTestResultsIntoScreenAndFile("21", "(double) Sum", T.CalculateAverageTime());

    T.ExecuteTest([dk, &di]() {
        for (int j = 0; j < 10000000; ++j) {
            dk[j] -= di;
    } });
    T.WriteTestResultsIntoScreenAndFile("22", "(double) Subtraction", T.CalculateAverageTime());

    T.ExecuteTest([dk, &di]() {
        for (int j = 0; j < 10000000; ++j) {
            dk[j] *= di;
    } });
    T.WriteTestResultsIntoScreenAndFile("23", "(double) Multiplication", T.CalculateAverageTime());

    T.ExecuteTest([dk, &di]() {
        for (int j = 0; j < 10000000; ++j) {
            dk[j] /= di;
    } });
    T.WriteTestResultsIntoScreenAndFile("24", "(double) Division", T.CalculateAverageTime());

    T.WriteMessageIntoScreenAndFile("|---------------------------------------------------\n|");

    T.ExecuteTest([]() {
        for (int j = 0; j < 10000000; ++j) {
            voidF();
    } });
    T.WriteTestResultsIntoScreenAndFile("25", "Call void function", T.CalculateAverageTime());

    T.ExecuteTest([&i]() {
        for (int j = 0; j < 10000000; ++j) {
            i = rand();
    } });
    T.WriteTestResultsIntoScreenAndFile("26", "Call rand()", T.CalculateAverageTime());

    T.ExecuteTest([&i]() {
        for (int j = 0; j < 10000000; ++j) {
            i = cos(j);
    } });
    T.WriteTestResultsIntoScreenAndFile("27", "Call cos()", T.CalculateAverageTime());

    T.ExecuteTest([&i]() {
        for (int j = 0; j < 10000000; ++j) {
            i = acos(j);
    } });
    T.WriteTestResultsIntoScreenAndFile("28", "Call acos()", T.CalculateAverageTime());

    T.ExecuteTest([&i]() {
        for (int j = 0; j < 10000000; ++j) {
            i = sqrt(j);
    } });
    T.WriteTestResultsIntoScreenAndFile("29", "Call sqrt()", T.CalculateAverageTime());

    T.ExecuteTest([&i]() {
        for (int j = 0; j < 10000000; ++j) {
            i = cbrt(j);
    } });
    T.WriteTestResultsIntoScreenAndFile("30", "Call cbrt()", T.CalculateAverageTime());
    * /
        T.WriteMessageIntoScreenAndFile("----------------------------------------------------");

    return 0;
}
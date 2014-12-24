/**
 * Random number creater
 */
#include <cstdint>
#include <cstdio>
#include <cmath>
#include <string>
#include <algorithm>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/scoped_array.hpp>
#include <boost/lexical_cast.hpp>
using std::string;
using std::generate;
using namespace::boost;

const string TRN_PROVIDER =
#ifdef WINDOWS
    R"(Microsoft Strong Cryptographic Provider)";
#else
    R"(/dev/urandom)"; // "/dev/random" is very slow
#endif


void create_random_number(const char rtype, const uint32_t total, const double param1, const double param2)
{
    random::rand48 r48(time(0));
    random::uniform_01<random::rand48&> u01(r48);
    scoped_array<double> results(new double[total]());
    switch (rtype) {
        case 'u': { //uniform
            random::uniform_real_distribution<double> mkr(param1, param2);
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 'n': { //normal
            random::normal_distribution<double> mkr(param1, param2);
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 'b': { //binomial
            random::binomial_distribution<int, double> mkr(param1, param2);
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 'c': { //cauchy
            random::cauchy_distribution<double> mkr(param1, param2);
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 'g': { //gamma
            random::gamma_distribution<double> mkr(param1, param2);
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 'p': { //poisson
            random::poisson_distribution<int, double> mkr(param1);
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 'o': { //geometric
            random::geometric_distribution<int, double> mkr;
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 'l': { //bernoulli
            random::bernoulli_distribution<double> mkr;
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 'e': { //exponential
            random::exponential_distribution<double> mkr(param1);
            std::generate(&results[0], &results[total], [&]() {return mkr(u01);});
            break;
        }
        case 't': { //true random number on uniform
            random::random_device rdevice(TRN_PROVIDER);
            random::uniform_real_distribution<double> mkr(param1, param2);
            std::generate(&results[0], &results[total], [&]() {return mkr(rdevice);});
            break;
        }
        default:
            break;
    }
    std::for_each(&results[0], &results[total], [](double v){printf("%.9lf\n", v);});
}


void driver(int argc, char* argv[])
{
    string   rtype;
    uint32_t total  = 0;
    double   param1 = 0.0;
    double   param2 = 0.0;

    auto check_args = [&]()->bool
    {
        if (argc != 5) {
            return false;
        }
        rtype = argv[1];
        if (rtype[0] != '-' or rtype.size() != 2) {
            return false;
        }
        bool valid_rtype = false;
        for (auto& s : string("unbelagocpt")) {
            if (s == rtype[1]) {
                valid_rtype = true;
            }
        }
        if (not valid_rtype or argv[2][0] == '-') {
            return false;
        }
        total  = lexical_cast<uint64_t>(argv[2]);
        param1 = lexical_cast<double>(argv[3]);
        param2 = lexical_cast<double>(argv[4]);
        if (0 == total) {
            return false;
        }
        return true;
    };

    try {
        if (not check_args()) {
            fprintf(stderr, "Usage: randomaker TYPE TOTAL ARG1 ARG2\n");
            fprintf(stderr, "Option TOTAL:\n  Total of random numbers to be created\n");
            fprintf(stderr, "Option TYPE:\n");
            fprintf(stderr, "  -u : Uniform distribution, ARG1 is range-left, ARG2 is range-right\n");
            fprintf(stderr, "  -n : Normal distribution, ARG1 is mean, ARG2 is sigma\n");
            fprintf(stderr, "  -b : Binomial distribution, ARG1 is targ, ARG2 is parg\n");
            fprintf(stderr, "  -e : Exponential distribution, ARG1 is lambda, ARG2 is useless\n");
            fprintf(stderr, "  -g : Gamma distribution, ARG1 is alpha, ARG2 is beta\n");
            fprintf(stderr, "  -l : Bernoulli distribution, ARG1 and ARG2 are useless\n");
            fprintf(stderr, "  -o : Geometric distribution, ARG1 and ARG2 are useless\n");
            fprintf(stderr, "  -c : Cauchy distribution, ARG1 is median, ARG2 is sigma\n");
            fprintf(stderr, "  -p : Poisson distribution, ARG1 is mean, ARG2 is useless\n");
            fprintf(stderr, "  -t : Same as -u, but creat True Random Number(non-pseudo)\n\n");
            quick_exit(EXIT_FAILURE);
        }
    }
    catch (std::exception& e) {
        fprintf(stderr, "arguments not correct!\n");
        return;
    }

    create_random_number(rtype[1], total, param1, param2);
    quick_exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[], char* env[])
{
    driver(argc, argv);
    return 0;
}





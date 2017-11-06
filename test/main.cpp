//
// Created by mbikola on 10/31/17.
//

#include "gtest/gtest.h"
#include <string>
#include <fstream>

#include "jh/jh.h"


/**
 * char 2 int macro.
 */
#define c2i(x) (( x>='0' && x<='9' ) ? x-'0' :  ( x>='a' && x<='f' ? 10+x-'a' : 10+x-'A'))

using namespace std;

// tuple of message => expected digest
typedef std::tr1::tuple<string, string> StringPair;


class Hex2BinTrait {
public:
    inline uint8_t *hex2bin( string input) {

        uint8_t * res = new uint8_t[input.length() / 2];
        memset(res, 0, input.length()/2);
        const char *i = input.c_str();
        int offset = 0;

        while(*i) { /// expecting it ends with '\0'
            res[offset++] = c2i(*i) << 4  | c2i(*(i+1));
            i+=2; // two chars per byte
        }

        return res;
    }

    inline string bin2hex( uint8_t *bin, size_t length ) {
        static const char hextable[] = "0123456789abcdef";
        string hex;

        hex.reserve(length * 2 + 1);
        size_t i = 0;

        while ( i < length ) {
            hex += hextable[ (bin[i] & 0x0f0) >> 4];
            hex += hextable[ bin[i] & 0x0f ];
            i++;
        }

        return hex;
    }
};


class HashShitFuncTestTemplate: public ::testing::TestWithParam<StringPair>, public Hex2BinTrait {
protected:

    size_t digest_size ;


    uint8_t *digest_binary = NULL;


    virtual void SetUp() {
        digest_binary = new uint8_t[digest_size];
    }
    virtual void TearDown() {
        if ( digest_binary ) delete[] digest_binary;

        digest_binary = NULL;
    }


    virtual string calc(string input) = 0;

};


class JhTest : public HashShitFuncTestTemplate {
public:
    JhTest() : HashShitFuncTestTemplate() { digest_size = 32; }; // jh256 requires 32bytes
protected:
    string calc(string input) {
        uint8_t *input_binary = hex2bin(input);
        jh256(input_binary, input.size()/2, digest_binary); ///, digest_size);
        delete[] input_binary;
        return bin2hex(digest_binary, digest_size);
    }
};

class JhBenchTest : public ::testing::Test, public Hex2BinTrait {
public:

protected:
    unsigned int calc(string input) {
        uint8_t digest_binary[32];
        const uint8_t ITERS = 255;
        uint8_t *input_binary = hex2bin(input);
        for ( unsigned int i = 0; i < input.size()/2; i++ ) {
            for (unsigned int j = 0 ; j<ITERS; j++ ) {
                input_binary[i] += j;
                jh256(input_binary, input.size() / 2, digest_binary); ///, digest_size);
            }
        }
        delete[] input_binary;
        return input.size()/2 * ITERS;
    }
};



TEST_P(JhTest, defaultFixturesTest) {
    string inputMessage, expectedDigest;
    std::tie(inputMessage, expectedDigest) = GetParam();

    string calculatedDigest = calc(inputMessage);

    EXPECT_EQ(calculatedDigest, expectedDigest);
}

#ifndef NDEBUG
TEST_F(JhBenchTest, benchmark) {
    clock_t t = clock();
    unsigned int iterations = calc("ее душа попадет в ад, ее будут рвать на куски, снова и снова, во тьме, среди криков отчаяния, целую вечность");
    const double work_time_per_hash = (clock() - t)  / iterations;
    ASSERT_TRUE(work_time_per_hash <= 4.0); // experimental value
}
#endif


TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
    // fuck life, the universe and everything related to cryptography and blockchains
    EXPECT_NE("42", "Answer to the Ultimate Question of Life, the Universe, and Everything");
}

class FixturesCSVLoader {
public:
    static vector<StringPair> load( const char *path_to_csv_file ) {


        ifstream infile(path_to_csv_file);
        assert(infile.good());

        string input, expected, line, digest;
        vector<StringPair> fixtures;

        while ( ! getline(infile, line).eof()) {
            if ( line[line.size()-1] == '\r')  {
                line.resize(line.size() - 1); // dirty hach against CRLF/CR/LF line ending handling in ::std::getline
            }

            stringstream ss(line);
            getline(ss, input, ',');
            getline(ss, expected, ',');

            assert(input.length() % 2 == 0);
            //ASSERT_EQ(input.length(), digest_size * 2);

            fixtures.push_back(StringPair(input, expected));
        }
        return fixtures;

    }
};

auto jhFixtures = FixturesCSVLoader::load("data/jh.csv");
INSTANTIATE_TEST_CASE_P(defaultFixturesTest, JhTest, ::testing::ValuesIn(jhFixtures));

int main(int args, char **argv) {
    ::testing::InitGoogleTest(&args, argv);

    return RUN_ALL_TESTS();
}

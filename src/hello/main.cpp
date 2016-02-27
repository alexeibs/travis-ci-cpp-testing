#include <string>

#include <QCoreApplication>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>
#include <cryptopp/twofish.h>

struct Greeter {
  virtual ~Greeter() {}
  virtual void sayHello(const std::string& name) = 0;
};

struct GreeterMock : Greeter {
  MOCK_METHOD1(sayHello, void(const std::string& name));
};

void sayHello(Greeter& greeter, const std::string& name) {
  greeter.sayHello(name);
}

const unsigned int YAPS_KEY_SIZE = CryptoPP::SHA256::DIGESTSIZE;
const unsigned int YAPS_NOISE_SIZE = 4096;
typedef byte Key[YAPS_KEY_SIZE];

void twofishEncrypt(const std::string& inputStd, std::string& outputStd, const Key& key) {
  byte iv[CryptoPP::Twofish::BLOCKSIZE];
  memset(iv, 0x01, CryptoPP::Twofish::BLOCKSIZE);

  CryptoPP::CFB_Mode<CryptoPP::Twofish>::Encryption cipher(key, YAPS_KEY_SIZE, iv);
  CryptoPP::StringSource(inputStd, true,
    new CryptoPP::StreamTransformationFilter(cipher,
      new CryptoPP::Base64Encoder(
        new CryptoPP::StringSink(outputStd), false
      ) // Base64Encoder
    ) // StreamTransformationFilter
  ); // StringSource
}

TEST(SayHello, BasicTest) {
  using ::testing::_;
  using ::testing::Return;

  GreeterMock greeterMock;
  EXPECT_CALL(greeterMock, sayHello(_))
      .WillOnce(Return());

  sayHello(greeterMock, "John");
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

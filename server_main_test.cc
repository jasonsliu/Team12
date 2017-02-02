#include "gtest/gtest.h"
#include "server_main.cc"


class NginxConfigParserTest : public ::testing::Test {
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
};



TEST(NginxServerTest, GetPort){
	NginxConfigParser parser;
	NginxConfig out_config;
	parser.Parse("example_config", &out_config);
	int port = getPort(out_config);
	bool success;
	if(port == 8080){
		success = true;
	}else{
		success == false;
	}

	EXPECT_TRUE(success) << "getPort function does not parse out port properly";
}


//TEST(NginxConfigParserTest, )


TEST_F(NginxConfigParserTest, NestedGetPort){
  std::string config_string = "server { listen 8070 ; }";
  std::stringstream config_stream(config_string);
  EXPECT_FALSE(parser.Parse(&config_stream, &out_config));
  int port = getPort(out_config);
  if(port == 8070)
  {
		success = true;
  }
  else
  {
		success == false;
  }
  EXPECT_TRUE(success) << "getPort function does not parse out port properly";
}



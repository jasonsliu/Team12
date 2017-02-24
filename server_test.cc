#include "gtest/gtest.h"
#include "server.h"


class ServerTest : public ::testing::Test {
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
};


TEST_F(ServerTest, getPort_from_config_file){
	parser.Parse("example_config", &out_config);
	int port = getPort(out_config);
	EXPECT_TRUE(port == 8080) << "MESSAGE: getPort got " << port<< std::endl;
}


TEST_F(ServerTest, getPort_from_server_block){
  std::string config_string = "server { listen 8070 ; }";
  std::stringstream config_stream(config_string);
  parser.Parse(&config_stream, &out_config);
  int port = getPort(out_config);
  EXPECT_TRUE(port == 8070) << "MESSAGE: getPort got " << port << std::endl;
}
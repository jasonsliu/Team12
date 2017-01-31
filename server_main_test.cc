#include "gtest/gtest.h"
#include "request_handler.h"

static int getPort (const NginxConfig &config) {
	for(const auto& statement : config.statements_) {
		bool kl = true;
		for (const std::string& token : statement->tokens_){
			if(kl){
				try { return stoi(token);} catch (...) {}
			}
			kl = (token != "listen");
		}
	}
	return -1;
}

static std::string getBasePath(const NginxConfig& config) {
	std::string pathfinder = "/";
	for(const auto& statement : config.statements_){
		bool kl = false;
		for(const std::string& token : statement->tokens_){
			kl = (token.find(pathfinder) != std::string::npos);
			if(kl){
				try { return token; } catch (...) {}
			}
		}
	}
	std::string s = "No valid path!";
	return s;
}

class NginxStringConfigTest : public ::testing::Test {
protected:
	NginxConfigParser parser_;
	NginxConfig config_;
	bool ParseString(const std::string& config_string) {
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &config_);
	}
};

TEST(NginxConfigParserTest, ExampleConfig){
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("example_config", &out_config);

	EXPECT_TRUE(success) <<"example_config not parsed successfully";
}

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

TEST(NginxConfigParserTest, MediaWikiConfig){
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("example_config", &out_config);

	EXPECT_TRUE (success) << "MediaWikiConfig file did not parse successfully";
}

//TEST(NginxConfigParserTest, )

TEST_F(NginxStringConfigTest, CurlyBracesConfig){
	bool success = ParseString("foo { bar;}");
	EXPECT_TRUE(success) << "Error with curly braces";
}

TEST_F(NginxStringConfigTest, SimpleConfig){
	bool success = ParseString ("foo bar;");
	EXPECT_TRUE(success) <<"one-line statement didn't parse";
}

TEST(NginxConfigParserTest, ParserConfig){
	NginxConfigParser parser;
	NginxConfig out_config;

	bool success = parser.Parse("example_config", &out_config);

	EXPECT_TRUE(success) << "config_parse file didn't parse successfully";
}

TEST(NginxServerTest, GetBasePath){
	NginxConfigParser parser;
	NginxConfig out_config;
	parser.Parse("example_config", &out_config);
	std::string base_path = getBasePath(out_config);
	bool success;
	if (base_path == "home/user/Team12"){
		success = true;
	}else{
		success = false;
	}

	EXPECT_TRUE(success) << "getBasePath function is not working properly";
}

TEST(NginxServerTest, EchoResponse){
  HttpResponse rep;
  HttpRequest req = {};
  bool success;
  std::string base_path = "/home/user/Team12";
  EchoHandler echoHand;
  std::map<std::string, std::string> handlerMap;
  handlerMap["directory"] = base_path;
  echoHand.EchoHandler::Init(handlerMap);
  echoHand.EchoHandler::HandleRequest(req, &rep);

  if (rep.headers_[1].second == "text/plain"){
    success = true;
  } else {
    success = false;
  }

  EXPECT_TRUE(success) << "Does not recognize echo server";
}

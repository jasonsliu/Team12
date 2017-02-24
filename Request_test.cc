#include "gtest/gtest.h"
#include "Constants.h"
#include "Request.h"
#include <string>


TEST(TestRequest, ParseEcho1){
    std::string s = "GET /ECHO/ HTTP/1.1\n";
    Request r(s.c_str());
    r.parse_request();

    std::string url = r.get_url();
    EXPECT_EQ(url, "/ECHO/") << "GTEST: Got " << url << std::endl;

    std::string reqmsg = r.get_req_msg();
    EXPECT_EQ(reqmsg, s) << "GTEST: Got " << reqmsg << std::endl;

    Http_method httpmethod = r.get_method();
    EXPECT_EQ(httpmethod, GET) << "GTEST: Got " << httpmethod << std::endl;

    Service_type type = r.get_type();
    EXPECT_EQ(type, ECHO_SERVICE) << "GTEST: Got " << type << std::endl;
}


TEST(TestRequest, ParseEcho2){
    std::string s = "post /ecHo/// garbage/garbage/";
    Request r(s.c_str());
    r.parse_request();
    
    std::string url = r.get_url();
    EXPECT_EQ(url, "/ecHo///") << "GTEST: Got " << url << std::endl;
    
    std::string reqmsg = r.get_req_msg();
    EXPECT_EQ(reqmsg, s) << "GTEST: Got " << reqmsg << std::endl;

    Http_method httpmethod = r.get_method();
    EXPECT_EQ(httpmethod, POST) << "GTEST: Got " << httpmethod << std::endl;

    Service_type type = r.get_type();
    EXPECT_EQ(type, ECHO_SERVICE) << "GTEST: Got " << type << std::endl;
}


TEST(TestRequest, ParseStatic1){
    std::string s = "GET /StaTic/foo/bar/foo/bar/foo/bar HTTP/1.1\n";
    Request r(s.c_str());
    r.parse_request();

    std::string url = r.get_url();
    EXPECT_EQ(url, "/StaTic/foo/bar/foo/bar/foo/bar") << "GTEST: Got " << url << std::endl;
    
    std::string reqmsg = r.get_req_msg();
    EXPECT_EQ(reqmsg, s) << "GTEST: Got " << reqmsg << std::endl;

    Http_method httpmethod = r.get_method();
    EXPECT_EQ(httpmethod, GET) << "GTEST: Got " << httpmethod << std::endl;

    Service_type type = r.get_type();
    EXPECT_EQ(type, STATIC_SERVICE) << "GTEST: Got " << type << std::endl;
    
    std::string file = r.get_file();
    EXPECT_EQ(file, "foo/bar/foo/bar/foo/bar") << "GTEST: Got " << file << std::endl;
}
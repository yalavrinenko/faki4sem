//
// Created by yalavrinenko on 04.04.23.
//

#include "src/CRTP.h"
#include <gtest/gtest.h>

class XmlParser : public Parser<XmlParser> {
public:
  void parse_() const { fmt::print("Parse from {}\n", __PRETTY_FUNCTION__); }
  void get_() const { fmt::print("Get from {}\n", __PRETTY_FUNCTION__); }

  std::string compile_() const {
    return "Xml Compile ret value";
  }
};

class JsonParser : public Parser<JsonParser> {
public:
  void parse_() const { fmt::print("Parse from {}\n", __PRETTY_FUNCTION__); }
  void get_() const { fmt::print("Get from {}\n", __PRETTY_FUNCTION__); }

  double compile_() const {
    return 3.14;
  }
};

template<typename Impl>
void parse(const Parser<Impl> &ps) {
  ps.common();
  ps.parse();
  ps.get();

  fmt::print("Compile result = {}\n", ps.compile());
}

TEST(CRTP, Xml) {
  XmlParser xml;
  parse(xml);
}

TEST(CRTP, Json) {
  JsonParser json;
  parse(json);
}
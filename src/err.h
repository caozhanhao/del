//   Copyright 2022 del - caozhanhao
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
#ifndef DEL_ERR_H
#define DEL_ERR_H
#include <stdexcept>
#include <string>
#include <iostream>
namespace del::except
{
  class Error : public std::runtime_error
  {
  public:
    Error(std::string details)
        : runtime_error(details)
    {}
  };
  void error(const std::string& str)
  {
    throw std::runtime_error("\033[31mERROR: \033[0m" + str);
  }
  void warn(const std::string& str)
  {
    std::cout << "\033[33mWARNING: \033[0m" << str << std::endl;
  }
}
#endif
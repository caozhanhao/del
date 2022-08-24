#include "err.h"
#include "option.h"
#include <cstdlib>
#include <string>
#include <filesystem>
#include <chrono>
auto enter_del()
{
  char const* home = getenv("HOME");
  if(home == nullptr) home = getenv("USERPROFILE");
  if(home == nullptr) home = getenv("HOMEDRIVE");
  if(home == nullptr) home = getenv("HOMEPATH");
  std::string h;
  if(home == nullptr)
  {
    del::except::warn("Can not find 'home'.Use '/del");
    h = '/';
  }
  else
    h = home;
  if(h.back() != '/') h += '/';
  std::filesystem::path del(h + ".del/");
  if (!std::filesystem::exists(del))
  {
    del::except::warn("Creating " + del.generic_string());
    std::filesystem::create_directory(del);
  }
  return del;
}
void warn_arg(const del::option::Option::CallbackArgType& vec)
{
  if(!vec.empty())
  {
    std::string warning = "Ignore arguments: ";
    for(auto& s : vec)
      warning += s + " ";
    del::except::warn(warning);
  }
}
int main(int argc, char* argv[])
{
  del::option::Option option(argc, argv);
  option.add(argv[0], [](del::option::Option::CallbackArgType args)
  {
    if(args.empty()) return;
    auto del = enter_del();
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp
        = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>
                                        (tp.time_since_epoch()).count());
    bool created = false;
    for (auto& r : args)
    {
      std::filesystem::path temp(r);
      if (!exists(temp))
      {
        del::except::error("No such file or directory");
        continue;
      }
      if(!created)
      {
        std::filesystem::create_directory(del.generic_string() + timestamp);
        created = true;
      }
      std::filesystem::rename(temp, std::filesystem::path(del.generic_string()
      + timestamp + "/" + temp.filename().string()));
    }
  });
  option.add("empty", [](del::option::Option::CallbackArgType args)
  {
    warn_arg(args);
    auto del = enter_del();
    if (!exists(del))
      create_directory(del);
    std::filesystem::directory_iterator list(del);
    for (auto& p : list)
    {
      remove_all(p);
    }
  });
  option.add("l", "list", [](del::option::Option::CallbackArgType args)
  {
    warn_arg(args);
    auto del = enter_del();
    for(auto& path : std::filesystem::directory_iterator(del))
    {
      auto name = path.path().filename().string();
      std::chrono::milliseconds timestamp = std::chrono::milliseconds(std::stoll(name));
      auto  time = std::chrono::system_clock::to_time_t(
          std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds>
          (timestamp));
      std::cout <<  "\033[32m" << std::put_time(std::localtime(&time), "%F %T") <<  "\033[0m\n";
      for(auto& r : std::filesystem::directory_iterator(path))
      {
        std::cout << r.path().filename().string() << std::endl;
      }
    }
  });
  option.parse().run();
  return 0;
}
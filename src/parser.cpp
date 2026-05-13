#include "parser.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

static void report_error(const std::string &line) {
  std::ofstream out("result.txt");
  out << line << "\n";
  out.close();
  std::exit(1);
}

static std::string read_line(std::ifstream &file) {
  std::string line;
  if (!std::getline(file, line)) {
    std::ofstream out("result.txt");
    out << "\n";
    out.close();
    std::exit(1);
  }
  return line;
}

static bool try_parse_int(const std::string &s, int &out) {
  if (s.empty())
    return false;
  for (char c : s)
    if (!std::isdigit(static_cast<unsigned char>(c)))
      return false;
  try {
    out = std::stoi(s);
    return true;
  } catch (...) {
    return false;
  }
}

static std::vector<std::string> split_space(const std::string &s) {
  std::vector<std::string> result;
  std::istringstream iss(s);
  std::string token;
  while (iss >> token)
    result.push_back(token);
  return result;
}

static std::vector<std::string> split_comma(const std::string &s) {
  std::vector<std::string> result;
  std::istringstream iss(s);
  std::string token;
  while (std::getline(iss, token, ','))
    result.push_back(token);
  return result;
}

static Resource parse_resource(const std::string &s, const std::string &line) {
  if (s == "iron")
    return Resource::IRON;
  if (s == "gold")
    return Resource::GOLD;
  if (s == "gems")
    return Resource::GEMS;
  if (s == "exp")
    return Resource::EXP;
  report_error(line);
  return Resource::IRON;
}

Simulator parse(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::ofstream out("result.txt");
    out << filename << "\n";
    out.close();
    std::exit(1);
  }

  std::string line = read_line(file);
  auto tokens = split_space(line);
  int N = 0;
  if (tokens.size() != 1 || !try_parse_int(tokens[0], N) || N < 1 || N > 255)
    report_error(line);

  struct RawRoom {
    std::vector<int> neighbors;
    int res[4] = {};
    std::string line;
  };
  std::vector<RawRoom> raw(N + 1);

  for (int expected = 0; expected <= N; ++expected) {
    line = read_line(file);
    tokens = split_space(line);
    if (tokens.size() != 2 && tokens.size() != 6)
      report_error(line);

    int id = 0;
    if (!try_parse_int(tokens[0], id) || id != expected)
      report_error(line);

    for (const auto &ns : split_comma(tokens[1])) {
      int nid = 0;
      if (!try_parse_int(ns, nid) || nid > N)
        report_error(line);
      raw[expected].neighbors.push_back(nid);
    }

    if (tokens.size() == 6) {
      for (int r = 0; r < 4; ++r)
        if (!try_parse_int(tokens[2 + r], raw[expected].res[r]) ||
            raw[expected].res[r] > 255)
          report_error(line);
    } else {
      for (int r = 0; r < 4; ++r)
        raw[expected].res[r] = 0;
    }

    raw[expected].line = line;
  }

  std::vector<std::set<int>> adj(N + 1);
  for (int i = 0; i <= N; ++i)
    for (int j : raw[i].neighbors)
      adj[i].insert(j);

  for (int i = 0; i <= N; ++i)
    for (int j : adj[i])
      if (adj[j].find(i) == adj[j].end())
        report_error(raw[j].line);

  std::vector<Room> rooms;
  rooms.reserve(N + 1);
  for (int i = 0; i <= N; ++i) {
    std::vector<int> neighbors(adj[i].begin(), adj[i].end());
    rooms.emplace_back(i, std::move(neighbors), raw[i].res);
  }

  line = read_line(file);
  tokens = split_space(line);
  int M = 0;
  if (tokens.size() != 2 || !try_parse_int(tokens[0], M) || M < 2 || M > 255)
    report_error(line);
  Resource target = parse_resource(tokens[1], line);

  return Simulator(std::move(rooms), M, target);
}

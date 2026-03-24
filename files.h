#ifndef FILES_H
#define FILES_H

#include <vector>
#include <string>

bool saveScores(const std::string& playerName, int points, const std::vector<int>& hideNumResult);
std::string getScore();

#endif
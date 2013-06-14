#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
#include <boost/tokenizer.hpp>

#define PROBLEM_CRS "asd.crs"
#define PROBLEM_STU "asd.stu"

void print_conflicts(std::vector< std::vector<int> > conflicts) {
  for (std::vector< std::vector<int> >::iterator i = conflicts.begin() ; i != conflicts.end(); ++i) {
    std::cout << "\n";
    for (std::vector<int>::iterator conflict = (*i).begin() ; conflict != (*i).end(); ++conflict)
      std::cout << ' ' << *conflict;
  }
}

class Timeslot
{
public:
  static int used_timeslots;
  int exams; // Total exams assigned to this Timeslot
  int id; // An identifier for the timeslot

  Timeslot() {
    exams = 0;
    used_timeslots++;
    id = used_timeslots;
  }
  ~Timeslot();
};
int Timeslot::used_timeslots = 0;

class Solution
{
public:
  int aptitude;

  // Each gen in the genotype saves the Timeslot to which the exam is assigned
  std::vector<Timeslot*> genotype;

  // Generates a random solution
  // @param n_exams Total number of exams present
  Solution(int n_exams) {
    genotype.resize(n_exams);

    for (std::vector<Timeslot*>::iterator exam = genotype.begin() ; exam != genotype.end(); ++exam) {

      // Fill every exam with it's own timeslot
      *exam = new Timeslot();
      (*exam)->exams++;
    }
  }

  void print() {
    for (std::vector<Timeslot*>::iterator exam = genotype.begin() ; exam != genotype.end(); ++exam) {
      std::cout << (*exam)->id << " ";
    }
  }
};

int main () {

  std::ifstream exams_file(PROBLEM_CRS);

  // Count the newlines to get the total of exams
  exams_file.unsetf(std::ios_base::skipws); // Don't skip newlines
  unsigned total_exams = std::count(
    std::istream_iterator<char>(exams_file),
    std::istream_iterator<char>(),
    '\n');

  std::vector< std::vector<int> > conflicts;

  conflicts.resize(total_exams);

  // Initializes the matrix
  for (int i = 0; i < total_exams; ++i)
    conflicts[i].resize(total_exams);

  // Read the students file to fill the conflicts matrix
  std::ifstream infile(PROBLEM_STU);

  boost::char_separator<char> sep(" "); // Exams separated by spaces

  // We parse every line of the file, filling the conflict matrix
  std::string line;
  while (std::getline(infile, line)) {

    // We tokenize the line and mark as conflicts every pair of exams
    // that appears in the same line
    std::istringstream iss(line);
    boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
    if (tokens.begin() != tokens.end()) { // Check if there is more than one exam
      for(boost::tokenizer< boost::char_separator<char> >::iterator exam = tokens.begin(); exam != tokens.end(); ++exam) {
        boost::tokenizer< boost::char_separator<char> >::iterator other_exam = exam;

        // We iterate on all the remaining exams in the line, after the current one
        // and mark the couple (current, next) and (next, current) as conflicted
        for(other_exam++; other_exam != tokens.end(); ++other_exam) {
          conflicts[atoi((*exam).c_str()) - 1][atoi((*other_exam).c_str()) - 1] = 1;
          conflicts[atoi((*other_exam).c_str()) - 1][atoi((*exam).c_str()) - 1] = 1;
        }
      }
    }
  }

  Solution sol (10);

  sol.print();

  return 0;
}
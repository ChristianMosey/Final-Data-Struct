#pragma once
#include <unordered_map>
using namespace std;

class Course {
	string id;
	string instructor;
	unsigned int credits;

public:
	bool operator==(const Course& rhs) const {
		return getId() == rhs.getId();
	}
	bool operator!=(const Course& rhs) const {
		return !(*this == rhs);
	}

	Course() {}
	Course(string _id, string _instructor, unsigned int _credits)
		: id(_id), instructor(_instructor), credits(_credits) {
	}

	string getId() const {
		return id;
	}

	string getInstructor() const {
		return instructor;
	}

	unsigned int getCredits() const {
		return credits;
	}
};

template<>
class hash<Course> {
public:
	size_t operator()(const Course& item) const {
		static hash<string> hf;
		return hf(item.getId());
	}
};
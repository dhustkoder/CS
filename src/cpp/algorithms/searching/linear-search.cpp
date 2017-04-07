#include <iostream>
#include <vector>
#include <string>
#include "common.hpp"
#include "linear-search.hpp"






int main(int argc, const char** argv)
{
	if (argc < 4) {
		std::cerr << "Usage: " << argv[0] << " [list] [target]\n";
		return EXIT_FAILURE;
	}

	std::vector<int> v = make_int_vector_from_strings(argv + 1, argc - 2);
	const int target = std::strtol(argv[argc - 1], NULL, 0);
	const auto itr = linear_search(v.cbegin(), v.cend(), target);

	if (itr != v.cend())
		std::cout << "Found " << target << " at " << (itr - v.cbegin()) << '\n';
	else
		std::cout << target << " not found\n";

	return EXIT_SUCCESS;
}


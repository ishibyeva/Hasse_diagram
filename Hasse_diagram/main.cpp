#include <iterator>
#include <sstream>
#include </home/irina/diploma/Hasse_diagram/Hasse_diagram/UserInterface.h>

std::vector<std::vector<size_t>> ReadDataFromFile(std::istream& input) {

	std::vector<std::vector<size_t>> result_data_vector;
	std::string line;
    while (std::getline(input, line))
    {
        std::istringstream line_stream(line);

        result_data_vector.emplace_back(std::istream_iterator<size_t>(line_stream),
                           std::istream_iterator<size_t>());
	}

	return result_data_vector;
}
int main() {
	std::cout << "Enter filename" << "\n";
	std::string filename;
	std::cin >> filename; 

	std::ifstream infile(filename);

	std::vector<std::vector<size_t>> vector_formatted_data = ReadDataFromFile(infile);

    Interface_KP kp_facets(vector_formatted_data);
	Interface_FR fr_facets(vector_formatted_data);

	fr_facets.ConvertToData();

    return 0;
}
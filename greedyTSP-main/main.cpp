#include "common.h"
#include "parameters.h"
#include "input.h"
#include "solver.h"


void checkSolution(const Solution &solution, const Instance &instance) {
    double computedLength = 0.0;
    for(size_t i = 0; i < solution.tour.size(); i++) {
        int from = solution.tour[i];
        int to = solution.tour[(i + 1) % solution.tour.size()];
        computedLength += instance.getDistance(from, to);
    }
    if(abs(computedLength - solution.totalLength) > eps) {
        std::cerr << "Solution length mismatch! Computed: " << computedLength << ", Reported: " << solution.totalLength << std::endl;
    } else {
        std::cout << "Solution length verified: " << solution.totalLength << std::endl;
    }
}
int main() {
    std::ifstream read_file_name("C:/Users/ORLab/OneDrive/Documents/Lab/greedyTSP-main/greedyTSP-main/fileNames.txt");
    if (!read_file_name.is_open()) {
        std::cerr << "Cannot open the file fileNames.txt" << std::endl;
        return 1;
    }

    std::string file_name;
    while (std::getline(read_file_name, file_name)) {
        if (file_name.empty()) continue;
        std::cout << "Processing Instance " << file_name << std::endl;
        Instance instance;
        instance.readFromFile("C:/Users/ORLab/OneDrive/Documents/Lab/greedyTSP-main/greedyTSP-main/formatted_data/" + file_name);

        Solver solver(instance);
        solver.setConfig_timeLimitInSeconds(60); 
        solver.setConfig_algorithm("ILS"); 

        auto start = std::chrono::high_resolution_clock::now();
        
        Solution bestSolution = solver.solve();
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << "Best tour length: " << bestSolution.totalLength << std::endl;
        std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
        checkSolution(bestSolution, instance);
        std::cout << "-----------------------------------------------------------------------" << std::endl;

    }

    read_file_name.close();
    return 0;
}
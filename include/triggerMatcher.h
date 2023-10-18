#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "DataStructures.h"

class triggerMatcher {
public:
        triggerMatcher(std::vector<double> &trigger_times, std::vector<ClusterDetector> &clusters) : m_trigger_times(trigger_times), m_clusters(clusters) {}

	void match_clusters(std::vector<double> &unmatched_trigger_times, std::vector<ClusterDetector> &unmatched_clusters, 
			    std::vector<double> &ambiguous_trigger_times, std::vector<ClusterDetector> &ambiguous_clusters, 
			    std::vector<std::pair<double,ClusterDetector>> &matched_pairs);

	void set_td_thesholds(std::pair<double,double> td_thresh) {
		td_min = td_thresh.first;
		td_max = td_thresh.second;
	}

private:
	bool is_a_match(int i, int j);
	bool is_unambiguous(int i, int j);

        /*double td_min = -7e6;
        double td_max = 20e6;*/

        double td_min = -500;
        double td_max = 150;
	
        std::vector<double> m_trigger_times;
	std::vector<ClusterDetector> m_clusters;
};

#include "../include/optics/optics.h"
#include <vector>



void clustering_test_1(){
	static const int N = 2;
	typedef std::array<double, N> point; //A list of N cartesian coordinates makes a point

    std::vector<point> points; //Your list of points goes here
	points = { {100,100}, {102,100}, {101,101},           //cluster 1
			   {-1,0}, {1,0}, {0,1},                     //cluster 2
			   {-100,-100}, {-102,-100}, {-101,-101}     //cluster 3
	};
	auto reach_dists = optics::compute_reachability_dists( points, 2, 10 );
	for( const auto& x : reach_dists){
        std::cout << x.to_string() << "; ";
	}

	auto clusters = optics::get_cluster_indices(reach_dists, 10);
	assert(clusters.size() == 3);
	assert( ( fplus::sort( clusters[0] ) == std::vector <std::size_t>({ 0,1,2 }) ) );
	assert( ( fplus::sort( clusters[1] ) == std::vector <std::size_t>({ 3,4,5 }) ) );
	assert( ( fplus::sort( clusters[2] ) == std::vector <std::size_t>({ 6,7,8 }) ) );

	double epsilon_est = optics::epsilon_estimation( points, 2 );
	std::cout << "Epsilon-Estimation: " << epsilon_est << std::endl;
}


void clustering_test_2() {
	static const int N = 2;
	typedef std::array<int, N> point; //A list of N cartesian coordinates makes a point

	std::vector<point> points; //Your list of points goes here
	points = { { 100,100 },{ 102,100 },{ 101,101 },           //cluster 1
	{ -1,0 },{ 1,0 },{ 0,1 },                     //cluster 2
	{ -100,-100 },{ -102,-100 },{ -101,-101 }     //cluster 3
	};
	auto reach_dists = optics::compute_reachability_dists( points, 2 );
	for ( const auto& x : reach_dists ) {
		std::cout << x.to_string() << "; ";
	}

	auto clusters = optics::get_cluster_indices( reach_dists, 2 );
	assert( clusters.size() == 3 );
	assert( (fplus::sort( clusters[0] ) == std::vector <std::size_t>( { 0,1,2 } )) );
	assert( (fplus::sort( clusters[1] ) == std::vector <std::size_t>( { 3,4,5 } )) );
	assert( (fplus::sort( clusters[2] ) == std::vector <std::size_t>( { 6,7,8 } )) );

	double epsilon_est = optics::epsilon_estimation( points, 2 );
    std::cout << "Epsilon-Estimation: " << epsilon_est << std::endl;

	optics::draw_reachability_plot( reach_dists, "ReachabilityPlot" );
}


void clustering_test_3(){
    static const int N = 2;
	typedef std::array<unsigned char, N> point; //A list of N cartesian coordinates makes a point

    std::vector<point> points; //Your list of points goes here
	points = { {100,100}, {102,100}, {101,101},           //cluster 1
			   {1,1}, {1,0}, {0,1},                     //cluster 2
			   {200,100}, {202,100}, {201,101}     //cluster 3
	};

    auto reach_dists = optics::compute_reachability_dists( points, 2, 10 );

    auto clusters = optics::get_cluster_indices( reach_dists, 10 );
	assert( clusters.size() == 3 );
	assert( (fplus::sort( clusters[0] ) == std::vector <std::size_t>( { 0,1,2 } )) );
	assert( (fplus::sort( clusters[1] ) == std::vector <std::size_t>( { 3,4,5 } )) );
	assert( (fplus::sort( clusters[2] ) == std::vector <std::size_t>( { 6,7,8 } )) );

	auto cluster_points = optics::get_cluster_points(reach_dists, 10, points);
	auto img = optics::draw_2d_clusters(cluster_points);
	img.save("Clusters2d");
}


void epsilon_estimation_test_1() {
	static const int N = 2;
	typedef std::array<double, N> point; //A list of N cartesian coordinates makes a point

	std::vector<point> points; //Your list of points goes here
	points = { { 0,0 },{ 1,0 },{ 0,1 },
	{ 10,0 },{ 0,10 },{ 6,6 },{ 4,4 },
	{ 10,10 },{ 9,10 },{ 10,9 }
	};

	double epsilon_est = optics::epsilon_estimation( points, 3 );
	assert( epsilon_est <3.090196 && epsilon_est >3.09019 );
}
void epsilon_estimation_test_2() {
	static const int N = 3;
	typedef std::array<double, N> point; //A list of N cartesian coordinates makes a point

	std::vector<point> points; //Your list of points goes here
	points = { { 0,0,0 },{ 1,0,0 },{ 0,0,1 },{ 0,1,0 },
	{ 5,0,0 },{ 0,5,0 },{ 0,0,5 },{ 5,5,5 }
	};

	double epsilon_est = optics::epsilon_estimation( points, 3 );
	assert( epsilon_est >2.236750 && epsilon_est <2.236751 );
}


void chi_test_1(){
    std::vector<optics::reachability_dist> reach_dists = {
        {1,10.0}, {2,9.0}, {3,9.0}, {4, 5.0},//SDA
		{5,5.49}, {6,5.0},//Cluster1
		{7, 6.5},//SUA
		{8,3.0},//SDA
		{9, 2.9}, {10, 2.8},//Cluster2
		{11, 10.0},{12, 12.0}//SUA
    };
    double chi = 0.1;
    std::size_t min_pts = 4;
   auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
   assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>({ {2, 5}, {0, 10}, { 6,10 } }) ) );
}
void chi_test_2() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 1,10.0 },{ 2,9.0 },{ 3,9.0 },{ 4, 5.0 },//SDA
		{ 5,5.49 },{ 6,5.0 },//Cluster1
		{ 7, 6.5 },//SUA
		{ 8,3.0 },//SDA
		{ 9, 2.9 },{ 10, 2.8 },//Cluster2
		{ 11, 10.0 },{ 12, 12.0 },//SUA
		{13, 4.0},//SDA
		{14, 4.1}, {15,4.0},{ 16,3.9 },//Cluster3
		{17,5.0}//SUA
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 2, 5 },{ 0, 10 },{ 6,10 }, {11,16} } )) );
}
void chi_test_3() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 1,11.0 },{ 2,9.0 },{ 3,9.0 },{ 4, 5.0 },//SDA
		{ 5,5.49 },{ 6,5.0 },//Cluster1
		{ 7, 6.5 },//SUA
		{ 8,3.0 },//SDA
		{ 9, 2.9 },{ 10, 2.8 },//Cluster2
		{ 11, 10.0 },{ 12, 10.0 },//SUA
		{ 13, 4.0 },//SDA
		{ 14, 4.1 },{ 15,4.0 },{ 16,3.9 },//Cluster3
		{ 17,12.0 }//SUA
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 2, 5 },{ 0, 9 },{ 6,10 },{0,16},{ 11,16 } } )) );
}
void chi_test_4() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 1,12.0 },{ 2,9.0 },{ 3,9.0 },{ 4, 5.0 },//SDA
		{ 5,5.49 },{ 6,5.0 },//Cluster1
		{ 7, 6.5 },//SUA
		{ 8,3.0 },//SDA
		{ 9, 2.9 },{ 10, 2.8 },//Cluster2
		{ 11, 10.0 },{ 12, 10.0 },//SUA
		{ 13, 4.0 },//SDA
		{ 14, 4.1 },{ 15,4.0 },{ 16,3.9 },//Cluster3
		{ 17,11.0 }//SUA
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 2, 5 },{ 0, 9 },{ 6,10 },{0,16},{ 11,16 } } )) );
}
void chi_test_5() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 1,12.0 },{ 2,9.0 },{ 3,9.0 },{ 4, 5.0 },//SDA
		{ 5,5.49 },{ 6,5.0 },//Cluster1
		{ 7, 6.5 },//SUA
		{ 8,3.0 },//SDA
		{ 9, 2.9 },{ 10, 2.8 },//Cluster2
		{ 11, 10.0 },{ 12, 10.0 },//SUA
		{ 13, 4.0 },//SDA
		{ 14, 4.1 },{ 15,4.0 },{ 16,3.9 },//Cluster3
		{ 17,12.0 }//SUA
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 2, 5 },{ 0, 9 },{ 6,10 },{0,16},{ 11,16 } } )) );
}
void chi_test_6() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 1,12.0 },{ 2,9.0 },{ 3,9.0 },{ 4, 5.0 },//SDA
		{ 5,5.49 },{ 6,5.0 },//Cluster1
		{ 7, 6.5 },//SUA
		{ 8,3.0 },//SDA
		{ 9, 2.9 },{ 10, 2.8 },//Cluster2
		{ 11, 10.0 },{ 12, 10.0 },//SUA
		{ 13, 4.0 },//SDA
		{ 14, 4.1 },{ 15,4.0 },{ 16,3.9 },//Cluster3
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 2, 5 },{ 0, 9 },{ 6,10 },{2,15},{ 11,15 } } )) );
}
void chi_test_7() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 1,12.0 },{ 2,9.0 },{ 3,9.0 },{ 4, 5.0 },//SDA
		{ 5,5.49 },{ 6,5.0 },//Cluster1
		{ 7, 11.0 },//SUA
		{ 8,3.0 },//SDA
		{ 9, 2.9 },{ 10, 2.8 },//Cluster2
		{ 11, 9.89 },{ 12, 9.89 },//SUA
		{ 13, 4.0 },//SDA
		{ 14, 4.1 },{ 15,4.0 },{ 16,3.9 },//Cluster3
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 0, 5 },{ 6, 9 },{6,15},{ 11,15 } } )) );
}
void chi_test_8() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 1,12.0 },{ 2,9.0 },{ 3,9.0 },{ 4, 5.0 },//SDA
		{ 5,5.49 },{ 6,5.0 },//Cluster1
		{ 7, 11.0 },//SUA
		{ 8,3.0 },//SDA
		{ 9, 2.9 },{ 10, 2.8 },//Cluster2
		{ 11, 9.89 },{ 12, 9.91 },//SUA
		{ 13, 4.0 },//SDA
		{ 14, 4.1 },{ 15,4.0 },{ 16,3.9 },//Cluster3
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 0, 5 },{ 6, 9 },{ 11,15 } } )) );
}
void chi_test_9() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 0, 5.0 }, { 1,5.49 },{ 2,5.0 },//Cluster1
		{ 3, 11.0 },//SUA
		{ 4,3.0 },//SDA
		{ 5, 2.9 },{ 6, 2.8 },//Cluster2
		{ 7, 9.89 },{ 8, 9.9 },//SUA
		{ 9, 4.0 },//SDA
		{ 10, 4.1 },{ 11,4.0 },{ 12,3.9 },//Cluster3
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 0, 2 },{ 3, 6 },{ 3,12 }, {8,12} } )) );
}
void chi_test_10() {
	std::vector<optics::reachability_dist> reach_dists = {
		{ 0, 5.0 }, { 1,5.49 },{ 2,5.0 },//Cluster1
		{ 3, 11.0 },//SUA
		{ 4,3.0 },//SDA
		{ 5, 2.9 },{ 6, 2.8 },//Cluster2
		{ 7, 9.89 },{ 8, 9.91 },//SUA
		{ 9, 4.0 },//SDA
		{ 10, 4.1 },{ 11,4.0 },{ 12,3.9 },//Cluster3
	};
	double chi = 0.1;
	std::size_t min_pts = 4;
	auto clusters = optics::get_chi_clusters( reach_dists, chi, min_pts );
	assert( (clusters == std::vector<std::pair<std::size_t, std::size_t>>( { { 0, 2 },{ 3, 6 }, {8,12} } )) );
}


void clustering_tests() {
	clustering_test_1();
	clustering_test_2();
	clustering_test_3();

	std::cout << "Clustering tests successful!" << std::endl;
}


void chi_cluster_tests(){
	chi_test_1();
	chi_test_2();
	chi_test_3();
	chi_test_4();
	chi_test_5();
	chi_test_6();
	chi_test_7();
	chi_test_8();
	chi_test_9();
	chi_test_10();

	std::cout << "Chi cluster extraction tests successful!" << std::endl;
}


void epsilon_estimation_tests(){
	epsilon_estimation_test_1();
	epsilon_estimation_test_2();

	std::cout << "Epsilon estimation tests successful!" << std::endl;
}


int main()
{
	epsilon_estimation_tests();
	chi_cluster_tests();
	clustering_tests();
}

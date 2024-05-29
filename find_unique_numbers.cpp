#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <iostream>


//#include <boost/static_assert.hpp>
//#include <boost/mpl/aux_/nested_type_wknd.hpp>
#include <boost/compute.hpp>
#include <boost/compute/types.hpp>
#include <boost/compute/type_traits.hpp>
#include <boost/compute/core.hpp>
#include <boost/compute/algorithm/sort.hpp>
#include <boost/compute/algorithm/find.hpp>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/algorithm/find_if_not.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/math.hpp>
#include <boost/compute/functional/logical.hpp>




namespace compute = boost::compute;


const int MAX_SIZE = 10000000;



template<typename T>
void vector_dump(std::vector<T>& v) {
	std::cout << "Vector dump:" << std::endl;
	for (auto e : v) {
		std::cout << e << ", ";
	}
	std::cout << std::endl << "Vector dump ended" << std::endl;
}


std::vector<int> find_uniq_numbers_GPU(std::vector<int>& src, boost::compute::context& context, boost::compute::command_queue& queue) {
	std::vector<int> result;

	//using compute::int4_;



	// create a vector on the device
	compute::vector<int> device_vector(src.size(), context);

	// transfer data from the host to the device
	compute::copy(
		src.begin(), src.end(), device_vector.begin(), queue
	);


	compute::sort(device_vector.begin(), device_vector.end(), queue);

	auto cur = device_vector[0];
	auto iterator = device_vector.begin();
	iterator++;

	while (iterator != device_vector.end()) {
		iterator++;
		int val = cur;
		auto pos = compute::find(iterator, device_vector.end(), val, queue);
		if (pos == device_vector.end()) {
			//found uniq

			result.push_back(reinterpret_cast<int32_t&>(cur));


			if (iterator != device_vector.end()) {
				cur = *iterator;
			}

		}
		else {
			//skip all repeated values

			using boost::compute::lambda::_1;
			auto skip_pos = device_vector.end();//compute::find_if_not(iterator, device_vector.end(), _1 % 2 == 1, queue);
			if (skip_pos != device_vector.end()) {
				iterator = skip_pos;


				cur = *skip_pos;
			}
			else {
				//there are repeated values only
				break;
			}
		}

	}

	return result;


}

std::vector<int32_t> find_uniq_numbers_CPU(std::vector<int32_t>& src) {
	std::vector<int32_t> result;

	std::sort(src.begin(), src.end());
	auto cur = src[0];
	auto iterator = src.begin();
	iterator++;

	while (iterator != src.end()) {
		iterator++;
		auto pos = std::find(iterator, src.end(), cur);
		if (pos == src.end()) {
			//found uniq
			result.push_back(cur);


			if (iterator != src.end()) {
				cur = *iterator;
			}

		}
		else {
			//skip all repeated values
			auto skip_pos = std::find_if_not(iterator, src.end(), [cur](int32_t value) { return value == cur; });
			if (skip_pos != src.end()) {
				iterator = skip_pos;


				cur = *skip_pos;
			}
			else {
				//there are repeated values only
				break;
			}
		}

	}

	return result;
}


int main() {


	// get default device and setup context
	compute::device device = compute::system::default_device();
	compute::context context(device);
	compute::command_queue queue(context, device);

	std::vector<int> tested_numbers(MAX_SIZE);
	std::iota(tested_numbers.begin(), tested_numbers.begin() + 1000, 0);
	std::fill(tested_numbers.begin() + 1000, tested_numbers.end(), -1);

	std::random_device rd;
	std::mt19937 g(rd());

	//std::shuffle(tested_numbers.begin(), tested_numbers.end(), g);

	auto uniq_elements_CPU = find_uniq_numbers_CPU(tested_numbers);
	auto uniq_elements_GPU = find_uniq_numbers_GPU(tested_numbers, context, queue);
	//vector_dump(uniq_elements);


}



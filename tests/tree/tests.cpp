#include "../../src/utki/debug.hpp"
#include "../../src/utki/tree.hpp"

#include "tests.hpp"


namespace{
void basic(){
	{
		utki::tree<int> t;

		t.children.push_back(35);

		ASSERT_ALWAYS(t.children.size() == 1)
	}

	{
		utki::tree<int> t(1,{34, 45});
		ASSERT_INFO_ALWAYS(t.value == 1, "t.value = " << t.value)
	}

	{
		const utki::tree<int> t{
			{34, 45},

			{
				{78, 89, 96},
				{32, 64, 128},
				decltype(t)(42, {98, 99, 100})
			}
		};

		ASSERT_ALWAYS(t.children.size() == 2)

		ASSERT_ALWAYS(t.children[0].children.size() == 2)
		ASSERT_ALWAYS(t.children[0].children[0].value == 34)
		ASSERT_ALWAYS(t.children[0].children[1].value == 45)

		ASSERT_ALWAYS(t.children[1].children.size() == 3)
		ASSERT_ALWAYS(t.children[1].children[0].children.size() == 3)
		ASSERT_ALWAYS(t.children[1].children[1].children.size() == 3)

		ASSERT_ALWAYS(t.children[1].children[2].children.size() == 3)
		ASSERT_ALWAYS(t.children[1].children[2].value == 42)

		ASSERT_ALWAYS(t.children[1].children[2].children[0].value == 98)
		ASSERT_ALWAYS(t.children[1].children[2].children[1].value == 99)
		ASSERT_ALWAYS(t.children[1].children[2].children[2].value == 100)
	}
}

void test_move_assignment(){
	utki::tree<int> t{
		{34, 45},

		{
			{78, 89, 96},
			{32, 64, 128},
			decltype(t)(42, {98, 99, 100})
		}
	};

	t.children[0].children = std::move(t.children[1].children);

	ASSERT_ALWAYS(t.children.size() == 2)

	ASSERT_ALWAYS(t.children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[1].children.size() == 3)

	ASSERT_ALWAYS(t.children[0].children[2].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[2].value == 42)

	ASSERT_ALWAYS(t.children[0].children[2].children[0].value == 98)
	ASSERT_ALWAYS(t.children[0].children[2].children[1].value == 99)
	ASSERT_ALWAYS(t.children[0].children[2].children[2].value == 100)

	ASSERT_ALWAYS(t.children[1].children.size() == 0)
}

void test_assignment(){
	utki::tree<int> t{
		{34, 45},

		{
			{78, 89, 96},
			{32, 64, 128},
			decltype(t)(42, {98, 99, 100})
		}
	};

	t.children[0].children = t.children[1].children;

	ASSERT_ALWAYS(t.children.size() == 2)

	ASSERT_ALWAYS(t.children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[1].children.size() == 3)

	ASSERT_ALWAYS(t.children[0].children[2].children.size() == 3)
	ASSERT_ALWAYS(t.children[0].children[2].value == 42)

	ASSERT_ALWAYS(t.children[0].children[2].children[0].value == 98)
	ASSERT_ALWAYS(t.children[0].children[2].children[1].value == 99)
	ASSERT_ALWAYS(t.children[0].children[2].children[2].value == 100)

	ASSERT_ALWAYS(t.children[1].children.size() == 3)
}

void test_comparison(){
	utki::tree<int> t{
		{34, 45},

		{
			{78, 89, 96},
			{32, 64, 128},
			decltype(t)(42, {98, 99, 100})
		}
	};


	ASSERT_ALWAYS(t.children.size() == 2)

	ASSERT_ALWAYS(t.children[1].children.size() == 3)
	ASSERT_ALWAYS(t.children[1].children[0].children.size() == 3)
	ASSERT_ALWAYS(t.children[1].children[1].children.size() == 3)

	ASSERT_ALWAYS(t.children[1].children[2].children.size() == 3)
	ASSERT_ALWAYS(t.children[1].children[2] == 42)

	ASSERT_ALWAYS(t.children[1].children[2].children[0] == 98)
	ASSERT_ALWAYS(t.children[1].children[2].children[1] == 99)
	ASSERT_ALWAYS(t.children[1].children[2].children[2] == 100)

	ASSERT_ALWAYS(t.children[1].children.size() == 3)
}

void test_constructors_from_container(){
	// lvalue, lvalue
	{
		typedef utki::tree<int> tree_int;
		tree_int::container_type children = {
			tree_int(10),
			tree_int(20)
		};
		ASSERT_ALWAYS(children.size() == 2)
		tree_int t(13, children);

		ASSERT_ALWAYS(t.value == 13)
		ASSERT_INFO_ALWAYS(t.children.size() == 2, "t.children.size() = " << t.children.size())
		ASSERT_INFO_ALWAYS(t.children[0].value == 10, "t.children[0].value = " << t.children[0].value)
		ASSERT_INFO_ALWAYS(t.children[1].value == 20, "t.children[1].value = " << t.children[1].value)
	}

	// lvalue, rvalue
	{
		typedef utki::tree<int> tree_int;
		tree_int::container_type children = {
			tree_int(10),
			tree_int(20)
		};
		ASSERT_ALWAYS(children.size() == 2)
		tree_int t(13, std::move(children));

		ASSERT_ALWAYS(t.value == 13)
		ASSERT_INFO_ALWAYS(t.children.size() == 2, "t.children.size() = " << t.children.size())
		ASSERT_INFO_ALWAYS(t.children[0].value == 10, "t.children[0].value = " << t.children[0].value)
		ASSERT_INFO_ALWAYS(t.children[1].value == 20, "t.children[1].value = " << t.children[1].value)
	}

	// rvalue, rvalue
	{
		typedef utki::tree<std::string> tree_str;
		tree_str::container_type children = {
			tree_str("10"),
			tree_str("20")
		};
		ASSERT_ALWAYS(children.size() == 2)
		std::string str("13");
		tree_str t(std::move(str), std::move(children));
		ASSERT_ALWAYS(str.length() == 0)

		ASSERT_ALWAYS(t.value == "13")
		ASSERT_INFO_ALWAYS(t.children.size() == 2, "t.children.size() = " << t.children.size())
		ASSERT_INFO_ALWAYS(t.children[0].value == "10", "t.children[0].value = " << t.children[0].value)
		ASSERT_INFO_ALWAYS(t.children[1].value == "20", "t.children[1].value = " << t.children[1].value)
	}
}

void test_traverser(){
	// non-const container
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traverser = utki::make_traverser(roots);

		ASSERT_INFO_ALWAYS(roots.begin()->value == 1, "roots.begin()->value = " << roots.begin()->value)

		ASSERT_INFO_ALWAYS(traverser.begin()->value == 1, "traverser.begin()->value = " << traverser.begin()->value)

		for(auto i = traverser.begin(); i != traverser.end(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100
		};

		if(encountered != expected){
			TRACE_ALWAYS(<< "encountered = ")
			for(auto& i : encountered){
				TRACE_ALWAYS(<< i << ", ")
			}
			TRACE_ALWAYS(<< std::endl)
			ASSERT_INFO_ALWAYS(false, "traversed tree is not as expected")
		}
	}

	// const container
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		for(auto& i : utki::make_traverser(roots)){
			encountered.push_back(i.value);
		}

		std::vector<int> expected{
			1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100
		};

		if(encountered != expected){
			TRACE_ALWAYS(<< "encountered = ")
			for(auto& i : encountered){
				TRACE_ALWAYS(<< i << ", ")
			}
			TRACE_ALWAYS(<< std::endl)
			ASSERT_ALWAYS(false)
		}
	}

	// non-const container, const_iterator
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traverser = utki::make_traverser(roots);

		ASSERT_INFO_ALWAYS(roots.begin()->value == 1, "roots.begin()->value = " << roots.begin()->value)

		ASSERT_INFO_ALWAYS(traverser.begin()->value == 1, "traverser.begin()->value = " << traverser.begin()->value)

		for(auto i = traverser.cbegin(); i != traverser.cend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100
		};

		if(encountered != expected){
			TRACE_ALWAYS(<< "encountered = ")
			for(auto& i : encountered){
				TRACE_ALWAYS(<< i << ", ")
			}
			TRACE_ALWAYS(<< std::endl)
			ASSERT_INFO_ALWAYS(false, "traversed tree is not as expected")
		}
	}

	// const container, const_iterator
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traverser = utki::make_traverser(roots);

		ASSERT_INFO_ALWAYS(roots.begin()->value == 1, "roots.begin()->value = " << roots.begin()->value)

		ASSERT_INFO_ALWAYS(traverser.begin()->value == 1, "traverser.begin()->value = " << traverser.begin()->value)

		for(auto i = traverser.cbegin(); i != traverser.cend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			1, 34, 45, 2, 3, 78, 89, 96, 4, 32, 64, 128, 42, 98, 99, 100
		};

		if(encountered != expected){
			TRACE_ALWAYS(<< "encountered = ")
			for(auto& i : encountered){
				TRACE_ALWAYS(<< i << ", ")
			}
			TRACE_ALWAYS(<< std::endl)
			ASSERT_INFO_ALWAYS(false, "traversed tree is not as expected")
		}
	}

	// non-const container, reverse_iterator
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traverser = utki::make_traverser(roots);

		ASSERT_INFO_ALWAYS(roots.begin()->value == 1, "roots.begin()->value = " << roots.begin()->value)

		ASSERT_INFO_ALWAYS(traverser.begin()->value == 1, "traverser.begin()->value = " << traverser.begin()->value)

		for(auto i = traverser.rbegin(); i != traverser.rend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1
		};

		if(encountered != expected){
			TRACE_ALWAYS(<< "encountered = ")
			for(auto& i : encountered){
				TRACE_ALWAYS(<< i << ", ")
			}
			TRACE_ALWAYS(<< std::endl)
			ASSERT_INFO_ALWAYS(false, "traversed tree is not as expected")
		}
	}

	// const container, reverse_iterator
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traverser = utki::make_traverser(roots);

		ASSERT_INFO_ALWAYS(roots.begin()->value == 1, "roots.begin()->value = " << roots.begin()->value)

		ASSERT_INFO_ALWAYS(traverser.begin()->value == 1, "traverser.begin()->value = " << traverser.begin()->value)

		for(auto i = traverser.rbegin(); i != traverser.rend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1
		};

		if(encountered != expected){
			TRACE_ALWAYS(<< "encountered = ")
			for(auto& i : encountered){
				TRACE_ALWAYS(<< i << ", ")
			}
			TRACE_ALWAYS(<< std::endl)
			ASSERT_INFO_ALWAYS(false, "traversed tree is not as expected")
		}
	}

	// non-const container, const_reverse_iterator
	{
		typedef utki::tree<int> tree;
		tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traverser = utki::make_traverser(roots);

		ASSERT_INFO_ALWAYS(roots.begin()->value == 1, "roots.begin()->value = " << roots.begin()->value)

		ASSERT_INFO_ALWAYS(traverser.begin()->value == 1, "traverser.begin()->value = " << traverser.begin()->value)

		for(auto i = traverser.crbegin(); i != traverser.crend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1
		};

		if(encountered != expected){
			TRACE_ALWAYS(<< "encountered = ")
			for(auto& i : encountered){
				TRACE_ALWAYS(<< i << ", ")
			}
			TRACE_ALWAYS(<< std::endl)
			ASSERT_INFO_ALWAYS(false, "traversed tree is not as expected")
		}
	}

	// const container, const_reverse_iterator
	{
		typedef utki::tree<int> tree;
		const tree::container_type roots{
			tree(1,{34, 45}),

			tree(2,{
				tree(3, {78, 89, 96}),
				tree(4,{32, 64, 128}),
				tree(42, {98, 99, 100})
			})
		};

		std::vector<int> encountered;

		auto traverser = utki::make_traverser(roots);

		ASSERT_INFO_ALWAYS(roots.begin()->value == 1, "roots.begin()->value = " << roots.begin()->value)

		ASSERT_INFO_ALWAYS(traverser.begin()->value == 1, "traverser.begin()->value = " << traverser.begin()->value)

		for(auto i = traverser.crbegin(); i != traverser.crend(); ++i){
			encountered.push_back(i->value);
		}

		std::vector<int> expected = {
			100, 99, 98, 42, 128, 64, 32, 4, 96, 89, 78, 3, 2, 45, 34, 1
		};

		if(encountered != expected){
			TRACE_ALWAYS(<< "encountered = ")
			for(auto& i : encountered){
				TRACE_ALWAYS(<< i << ", ")
			}
			TRACE_ALWAYS(<< std::endl)
			ASSERT_INFO_ALWAYS(false, "traversed tree is not as expected")
		}
	}
}
}


void test_utki_tree(){
	basic();
	test_assignment();
	test_move_assignment();
	test_comparison();
	test_constructors_from_container();
	test_traverser();
}

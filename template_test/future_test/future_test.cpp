
#include <iostream>
#include "future_utility.h"

class TestWhenAllException : public ExceptionBase {
public:
	const char* what() const noexcept { return "test when all"; }
};

void Test1() {
	std::cout << std::endl << "Test1 start......" << std::endl;
	auto flag = std::make_shared<bool>(false);
	auto fut = MakeReadyFuture<uint32_t>(20190001);

	WhenAll(std::move(fut)).Then([flag](std::tuple<Future<uint32_t>>&& result) {
		auto fut = std::move(std::get<0>(result));
		std::cout << "1-EXPECT_TRUE(fut.is_ready()):" << fut.is_ready() << std::endl;
		std::cout << "1-EXPECT_TRUE(std::get<0>(fut.GetValue()) == 20190001):" 
			<< std::get<0>(fut.GetValue()) << std::endl;
		*flag = true;
		return MakeReadyFuture<>();
	});
	std::cout << "1-EXPECT_TRUE(*flag):" << *flag << std::endl;
}

void Test2() {
	std::cout << std::endl << "Test2 start......" << std::endl;
	auto flag = std::make_shared<bool>(false);
	auto fut1 = MakeReadyFuture<uint32_t>(20190001);
	auto fut2 = MakeReadyFuture<std::string>("hello");
	auto fut3 = MakeReadyFuture<>();

	WhenAll(std::move(fut1), std::move(fut2), std::move(fut3))
		.Then([flag](std::tuple<Future<uint32_t>, Future<std::string>, Future<>>&& result) {
		auto fut1 = std::move(std::get<0>(result));
		std::cout << "2-EXPECT_TRUE(fut1.is_ready()):" << fut1.is_ready() << std::endl;
		std::cout << "2-EXPECT_TRUE(std::get<0>(fut1.GetValue()) == 20190001):"
			<< std::get<0>(fut1.GetValue()) << std::endl;

		auto fut2 = std::move(std::get<1>(result));
		std::cout << "2-EXPECT_TRUE(fut2.is_ready()):" << fut2.is_ready() << std::endl;
		std::cout << "2-EXPECT_TRUE(std::get<0>(fut2.GetValue()) == \"hello\"):" << std::get<0>(fut2.GetValue()) << std::endl;

		auto fut3 = std::move(std::get<2>(result));
		std::cout << "2-EXPECT_TRUE(fut3.is_ready()):" << fut3.is_ready() << std::endl;
		*flag = true;
		return MakeReadyFuture<>();
			});
	std::cout << "2-EXPECT_TRUE(*flag):" << *flag << std::endl;
}

void Test3() {
	std::cout << std::endl << "Test3 start......" << std::endl;
	auto flag = std::make_shared<bool>(false);
	auto pr1 = Promise<uint32_t>();
	auto pr2 = Promise<uint32_t>();
	auto pr3 = Promise<uint32_t>();
	auto fut1 = pr1.get_future();
	auto fut2 = pr2.get_future();
	auto fut3 = pr3.get_future();

	//WhenAll会先检查所有的变量是否设置好了
	//1：如果设置好了直接执行Then中的函数指针
	//2：没有没设置好，就相当于Then中的函数指针只是设置了一个回调
	//然后每次pr调用SetException/SetValue的时候会检查所有的pr值是否都设置了
	//如果所有pr的值都设置了，则调用Then中设置的回调函数
	//尼玛这个真是复杂---------
	WhenAll(std::move(fut1), std::move(fut2), std::move(fut3))
		.Then([flag](std::tuple<Future<uint32_t>, Future<uint32_t>, Future<uint32_t>>&& result) {
		auto fut1 = std::move(std::get<0>(result));
		std::cout << "3-EXPECT_TRUE(fut1.is_ready()):" << fut1.is_ready() << std::endl;
		std::cout << "3-EXPECT_TRUE(std::get<0>(fut1.GetValue()) == 1):" 
			<< std::get<0>(fut1.GetValue()) << std::endl;

		auto fut2 = std::move(std::get<1>(result));
		std::cout << "3-EXPECT_TRUE(fut2.is_failed()):" << fut2.is_failed() << std::endl;
		std::cout << "3-EXPECT_TRUE(fut2.GetException().is(TestWhenAllException())):"
			<< fut2.GetException().is(TestWhenAllException()) << std::endl;

		auto fut3 = std::move(std::get<2>(result));
		std::cout << "3-EXPECT_TRUE(fut3.is_ready()):" << fut3.is_ready() << std::endl;
		std::cout << "3-EXPECT_TRUE(std::get<0>(fut3.GetValue()) == 3):"
			<< std::get<0>(fut3.GetValue()) << std::endl;
		*flag = true;
		return MakeReadyFuture<>();
			});
	std::cout << "3-EXPECT_FALSE(*flag):" << *flag << std::endl;
	pr2.SetException(TestWhenAllException());
	std::cout << "3-EXPECT_FALSE(*flag):" << *flag << std::endl;
	pr3.SetValue(3);
	std::cout << "3-EXPECT_FALSE(*flag):" << *flag << std::endl;
	pr1.SetValue(1);

	std::cout << "3-EXPECT_TRUE(*flag):" << *flag << std::endl;
}

void Test4() {
	std::cout << std::endl << "Test4 start......" << std::endl;
	auto                          flag = std::make_shared<bool>(false);
	auto                          pr1 = Promise<uint32_t>();
	auto                          pr2 = Promise<uint32_t>();
	auto                          pr3 = Promise<uint32_t>();
	std::vector<Future<uint32_t>> input;
	input.push_back(pr1.get_future());
	input.push_back(pr2.get_future());
	input.push_back(pr3.get_future());

	WhenAll(input.begin(), input.end()).Then([flag](std::vector<Future<uint32_t>>&& result) {
		auto fut1 = std::move(result[0]);
		std::cout << "4-EXPECT_TRUE(fut1.is_ready()):" << fut1.is_ready() << std::endl;
		std::cout << "4-EXPECT_TRUE(std::get<0>(fut1.GetValue()) == 1):"
			<< std::get<0>(fut1.GetValue()) << std::endl;

		auto fut2 = std::move(result[1]);
		std::cout << "4-EXPECT_TRUE(fut2.is_failed()):" << fut2.is_failed() << std::endl;
		std::cout << "4-EXPECT_TRUE(fut2.GetException().is(TestWhenAllException())):"
			<< fut2.GetException().is(TestWhenAllException()) << std::endl;

		auto fut3 = std::move(result[2]);
		std::cout << "4-EXPECT_TRUE(fut3.is_ready()):" << fut3.is_ready() << std::endl;
		std::cout << "4-EXPECT_TRUE(std::get<0>(fut3.GetValue()) == 3):"
			<< std::get<0>(fut3.GetValue()) << std::endl;
		*flag = true;
		return MakeReadyFuture<>();
		});
	std::cout << "4-EXPECT_FALSE(*flag):" << *flag << std::endl;
	pr2.SetException(TestWhenAllException());
	std::cout << "4-EXPECT_FALSE(*flag):" << *flag << std::endl;
	pr3.SetValue(3);
	std::cout << "4-EXPECT_FALSE(*flag):" << *flag << std::endl;
	pr1.SetValue(1);

	std::cout << "3-EXPECT_TRUE(*flag):" << *flag << std::endl;
}

void Test5() {
	std::cout << std::endl << "Test5 start......" << std::endl;
	auto flag = std::make_shared<bool>(false);
	auto reach = std::make_shared<bool>(false);
	auto counter = std::make_shared<uint32_t>(false);

	DoUntil([flag]() { return *flag; },
		[flag, counter]() {
			(*counter)++;
			if (*counter == 12) {
				*flag = true;
			}
			if (*counter == 7) {
				return MakeExceptionFuture<>(TestWhenAllException());
			}
			return MakeReadyFuture<>();
		})
		.Then([reach]() {
			std::cout << "5-ADD_FAILURE(): failed........." << std::endl;
			return MakeReadyFuture<>();
			});
		std::cout << "5-EXPECT_FALSE(*flag):" << *flag << std::endl;
		std::cout << "5-EXPECT_FALSE(*reach):" << *reach << std::endl;
		std::cout << "5-EXPECT_TRUE(*counter == 7):" << *counter << std::endl;
}

int main() {
	Test1();

	Test2();

	Test3();

	Test4();

	Test5();
	return 0;
}

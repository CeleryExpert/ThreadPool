#ifndef ANY_H
#define ANY_H
#include<memory>

class Any {
public:
	Any() = default;
	Any(const Any&) = delete;
	Any& operator=(const Any&) = delete;
	Any(Any&& other) = default;
	Any& operator=(Any&&) = default;
	template<typename T>
	Any(T&& data) {
		base_ = std::make_unique<Derived<T>>(std::forward<T>(data));
	}
	template<typename T>
	T cast_() {
		Derived<T>* derived = dynamic_cast<Derived<T>*>(base_.get());
		if (derived == nullptr) {
			throw "type is un match";
		}
		return derived->Data;
	}
private:
	class Base {
	public:
		virtual ~Base() {}
	};
	template<typename T>
	class Derived : public Base {
	public:
		Derived(const T&
			data) : Data(data) {
		}
		T Data;
	};
private:
	std::unique_ptr<Base> base_;
};
#endif // !ANY_H



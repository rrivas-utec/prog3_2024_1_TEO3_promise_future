#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>

struct message_t {
  std::string action;
  int oper1;
  int oper2;
};
void task_messager_1(std::promise<message_t> prm, std::future<int> fut) {
  std::cout << "Procesando...\n";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Enviando mensaje...\n";
  message_t msg;
  std::cout << "ingrese la accion (sum, mul): ";
  std::cin >> msg.action;
  std::cout << "ingrese el operador #1: ";
  std::cin >> msg.oper1;
  std::cout << "ingrese el operador #2: ";
  std::cin >> msg.oper2;
  prm.set_value(msg);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "El resultado obtenido es:" << fut.get() << std::endl;
}
void task_messager_2(std::future<message_t> fut, std::promise<int> prm) {
  std::this_thread::sleep_for(std::chrono::seconds(4));
  auto msg = fut.get();
  int resultado = 0;
  if (msg.action == "sum") {
    resultado = msg.oper1 + msg.oper2;
  }
  else if (msg.action == "mul") {
    resultado = msg.oper1 * msg.oper2;
  }
  std::cout << "Enviando resultado...."<< std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(4));
  prm.set_value(resultado);
}

void ejemplo_1() {
  // promise/future #1
  std::promise<message_t> prm_1;
  std::future<message_t> fut_1 = prm_1.get_future();

  // promise/future #2
  std::promise<int> prm_2;
  std::future<int> fut_2 = prm_2.get_future();

  std::jthread t1(task_messager_1, std::move(prm_1), std::move(fut_2));
  std::jthread t2(task_messager_2, std::move(fut_1), std::move(prm_2));

  t1.join();
  t2.join();
}

template<typename Iterator, typename T = typename Iterator::value_type>
auto sumarizar_vector(Iterator start, Iterator stop) {
  return std::accumulate(start, stop, T{});
}
template <typename Iterator, typename T = typename Iterator::value_type>
void ejemplo_2(Iterator start, Iterator stop) {
  auto total = std::async(sumarizar_vector<Iterator, T>, start, stop);
  std::cout << total.get() << std::endl;
}

auto sumar(int a, int b) {
  return a + b;
}

void ejemplo_3() {
  auto total = std::async(sumar, 10, 20);
  std::cout << total.get();
}

int main() {
  std::vector<int> v(10);
  std::iota(v.begin(), v.end(), 1);
  ejemplo_2(v.begin(), v.end());
//  ejemplo_3();
  return 0;
}

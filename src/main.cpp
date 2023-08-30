#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <vector>
#include <fmt/core.h>

int main()
{
    // Declare a pool of 8 worker threads:
    exec::static_thread_pool pool(8);

    // Get a handle to the thread pool:
    auto sched = pool.get_scheduler();

    // Describe some work:
    // Creates 3 sender pipelines that are executed concurrently by passing to `when_all`
    // Each sender is scheduled on `sched` using `on` and starts with `just(n)` that creates a Sender that just forwards `n` to the next sender.
    // After `just(n)`, we chain `then(fun)` which invokes `fun` using the value provided from `just()`
    // Note: No work actually happens here. Everything is lazy and `work` is just an object that statically
    // represents the work to later be executed
    auto fun = [](int i)     { return i*i; };
    auto work = stdexec::when_all(
            stdexec::on(sched, stdexec::just(0) | stdexec::then(fun)),
            stdexec::on(sched, stdexec::just(1) | stdexec::then(fun)),
            stdexec::on(sched, stdexec::just(2) | stdexec::then(fun))
    );

    // Launch the work and wait for the result
    auto [i, j, k] = stdexec::sync_wait(std::move(work)).value();

    // Print the results:
    fmt::print("{} {} {}\n", i, j, k);


    stdexec::sender auto begin = stdexec::schedule(sched);
    stdexec::sender auto hi = stdexec::then(begin, [] {
        fmt::print("Have an int: \n");
        return 12;
    });

    auto add_42 = stdexec::then(hi, [] (int arg) {return arg + 42;});

    auto [ii] = stdexec::sync_wait(add_42).value();

    fmt::print("salida: {}\n", ii);

}

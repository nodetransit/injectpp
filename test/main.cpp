#include <injectpp.hpp>
#include "test.hpp"


struct Service2
{
    int a = 1;
};

struct Service3
{
    int a = 2;
};

struct Service1
{
    Service1(Service2* s2_, Service3* s3_)
          :
          s2(s2_),
          s3(s3_)
    {
    };

    Service2* s2;
    Service3* s3;
};

std::unique_ptr<Service1>
service1Factory(Service2* s2, Service3* s3)
{
    return std::make_unique<Service1>(s2, s3);
}

std::unique_ptr<Service2>
service2Factory()
{
    return std::make_unique<Service2>();
}

std::unique_ptr<Service3>
service3Factory()
{
    return std::make_unique<Service3>();
}

void
print_service(Service1* s)
{
    std::cout << s->s2->a << std::endl;
    std::cout << s->s3->a << std::endl;
}

void
demo_dependency_injection()
{
    nt::ioc::Config cfg;
    cfg.add(service1Factory);
    cfg.add(service2Factory);
    cfg.add(service3Factory);
    nt::ioc::Injector i = cfg.build_injector();

    Service1* s = i.get_instance<Service1>();
    print_service(s);

    puts("");

    std::unique_ptr<Service1> s2 = i.inject(service1Factory);
    print_service(s2.get());
}

int
_main(
      int argc,
      char** argv)
{
    try {
        demo_dependency_injection();
    }
    catch (const std::runtime_error& err) {
        std::cerr << "Runtime error: " << err.what() << std::endl;
    }

    return 0;
}

//----------------------------------------------------------------------

namespace test {

class IController
{
public:
    virtual void
    control() = 0;

    virtual ~IController()
    { }
};

class Input
{
public:
    Input()
    {
    }

    void get();
};

class Config
{
public:
    Config();

    intptr_t getInstance();
};

class Service
{
private:
    Input * input;
    Config* config;
public:
    Service(Input* input, Config* config)
          :
          input(input),
          config(config)
    {
    }

    void
    serve();
};

void
Input::get()
{
    std::cout << "getting" << std::endl;
}

void
Service::serve()
{
    std::cout << "serving" << std::endl;
    std::cout << "inputting ";
    input->get();
    std::cout << std::endl;
}

class Controller :
      public IController
{
private:
    Service* service;
    Input  * input;
    Config * config;
public:
    Controller(Service* service, Input* input, Config* config);

    ~Controller()
    { }

    void
    control();
};

Controller::Controller(
      Service* service,
      Input* input,
      Config* config)
      :
      service(service),
      input(input),
      config(config)
{
}

void
Controller::control()
{
    service->serve();
    input->get();
}

Config::Config()
{
}

intptr_t
Config::getInstance()
{
    return reinterpret_cast<intptr_t>(this);
}
}

namespace cx {
std::unique_ptr<test::Controller>
ControllerFactory(test::Service* s, test::Input* i, test::Config* c)
{
    return std::make_unique<test::Controller>(s, i, c);
}

std::unique_ptr<test::Service>
ServiceFactory(test::Input* i, test::Config* c)
{
    return std::make_unique<test::Service>(i, c);
}

std::unique_ptr<test::Input>
InputFactory()
{
    return std::make_unique<test::Input>();
}

std::unique_ptr<test::Config>
ConfigFactory()
{
    return std::make_unique<test::Config>();
}
}

void
runController(test::IController* c)
{
    std::cout << typeid(c).name() << std::endl;
    c->control();
}

nt::ioc::Config
createContainerConfig()
{
    nt::ioc::Config cfg;

    cfg.add(cx::ControllerFactory);
    // cfg.add([](test::Service* s, test::Input* i) { return std::make_unique<test::Controller>(s, i); });
    cfg.add(cx::ServiceFactory);
    cfg.add([] { return std::make_unique<test::Input>; }());
    cfg.add(cx::ConfigFactory);

    return cfg;
}

void
run()
{
    nt::ioc::Config   cfg = createContainerConfig();
    nt::ioc::Injector i   = cfg.build_injector();

    auto* cc = i.get_instance<test::Controller>();
    std::cout << typeid(cc).name() << std::endl;

    auto* c = i.get_instance<test::Controller, test::IController>();

    runController(c);
}

void
run2()
{
    nt::ioc::Config   cfg = createContainerConfig();
    nt::ioc::Injector i   = cfg.build_injector();

    auto* cc = i.get_instance<test::Controller>();
    std::cout << typeid(cc).name() << std::endl;

    auto* c = i.get_instance<test::Controller, test::IController>();
    auto* d = i.get_instance<test::Controller, test::IController>();

    runController(c);
    runController(d);
}

void
run3()
{
    nt::ioc::Config   cfg1 = createContainerConfig();
    nt::ioc::Injector i1   = cfg1.build_injector();

    auto* cc = i1.get_instance<test::Controller>();
    std::cout << typeid(cc).name() << std::endl;

    nt::ioc::Config   cfg2 = createContainerConfig();
    nt::ioc::Injector i2   = cfg2.build_injector();

    auto* c = i2.get_instance<test::Controller, test::IController>();
    auto* d = i2.get_instance<test::Controller, test::IController>();

    runController(c);
    runController(d);
}

int
main(
      int argc,
      char** argv)
{
    try {
        run();
        run2();
        run3();
        run();
        run2();
        run3();
    } catch (const std::runtime_error& err) {
        std::cerr << "Runtime error: " << err.what() << std::endl;
    }

    return EXIT_SUCCESS;
}

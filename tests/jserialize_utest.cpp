#include "shared/defmac.h"
#include "shared/list.h"
#include "shared/clife_base.h"
#include "shared/clife_ptr.h"
#include "shared/logger/logger.h"
#include "shared/logger/config.h"
#include "shared/qt/logger/logger_operators.h"
#include "shared/qt/quuidex.h"
#include "shared/qt/communication/commands_base.h"
#include "shared/qt/communication/commands_pool.h"
#include "shared/qt/communication/serialization/json.h"

using namespace std;

namespace communication {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = CommandsPool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = CommandsPool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(TestCommand1,     "5b3f0fb8-61f6-43e5-9042-f15e4d9234a6")
REGISTRY_COMMAND_SINGLPROC(TestCommand2,     "47b0c37d-ebd5-47d8-9eca-9612a5ccb384")
REGISTRY_COMMAND_SINGLPROC(TestCommand3,     "1467ad26-317f-4eed-a512-3b819b9b25fb")
REGISTRY_COMMAND_SINGLPROC(TestCommand4,     "48a1a5cd-6d44-42a6-b71f-4ad346589f43")
REGISTRY_COMMAND_SINGLPROC(TestCommand5,     "0544afa8-344e-4183-9431-cf4d9e3c6b3b")
REGISTRY_COMMAND_SINGLPROC(TestCommand6,     "936d0dca-1bbc-4689-8650-69de6396f4d2")
REGISTRY_COMMAND_SINGLPROC(TestCommand7,     "704f8d93-5350-4712-91e4-47dbcf004911")
REGISTRY_COMMAND_SINGLPROC(TestCommand8,     "4eef6815-b4f3-408d-9234-34484f4d00c9")


#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC
} // namespace command

namespace data {

struct TestCommand2 : Data<&command::TestCommand2,
                            Message::Type::Command>
{
    qint32 a = {1};
    qint64 b = {2};

    DECLARE_J_SERIALIZE_FUNC
};
template <typename Packer>
Packer& TestCommand2::jserialize(Packer& p)
{
    p.startObject();
    p.member("a") & a;
    p.member("b") & b;
    return p.endObject();
}

struct TestCommand3 : Data<&command::TestCommand3,
                            Message::Type::Command>
{
    qint32 c = {3};
    TestCommand2 command2;

    DECLARE_J_SERIALIZE_FUNC
};
template <typename Packer>
Packer& TestCommand3::jserialize(Packer& p)
{
    p.startObject();
    p.member("c") & c;
    p.member("command2") & command2;
    return p.endObject();
}

struct TestCommand4 : Data<&command::TestCommand4,
                            Message::Type::Command>
{
    qint32 d = {10};
    QVector<int> v;

    DECLARE_J_SERIALIZE_FUNC
};
template <typename Packer>
Packer& TestCommand4::jserialize(Packer& p)
{
    p.startObject();
    p.member("d") & d;
    p.member("v") & v;
    return p.endObject();
}

struct TestCommand5 : Data<&command::TestCommand5,
                            Message::Type::Command>
{
    qint32 f = {0};
    QVector<TestCommand2> v;

    DECLARE_J_SERIALIZE_FUNC
};
template <typename Packer>
Packer& TestCommand5::jserialize(Packer& p)
{
    p.startObject();
    p.member("f") & f;
    p.member("v") & v;
    return p.endObject();
}

struct Point : clife_base
{
    typedef clife_ptr<Point> Ptr;

    int x = {5};
    int y = {6};

    template <typename Packer>
    Packer& jserialize(Packer& p)
    {
        p.startObject();
        p.member("x") & x;
        p.member("y") & y;
        return p.endObject();
    }
};

struct Point3
{
    typedef clife_ptr<Point3> Ptr;

    int x = {0};
    int y = {0};
    int z = {0};

    template <typename Packer>
    Packer& jserialize(Packer& p)
    {
        p.startObject();
        p.member("x") & x;
        p.member("y") & y;
        p.member("z") & z;
        return p.endObject();
    }
};

struct TestCommand6 : Data<&command::TestCommand6,
                            Message::Type::Command>
{
    qint32 e = {0};
    Point::Ptr point;
    qint32 e2 = {0};
    qint32 e3 = {0};

    DECLARE_J_SERIALIZE_FUNC
};
template <typename Packer>
Packer& TestCommand6::jserialize(Packer& p)
{
    p.startObject();
    p.member("e") & e;
    p.member("point") & point;
    p.member("e2") & e2;
    p.member("e3") & e3;
    return p.endObject();
}

struct TestCommand7 : Data<&command::TestCommand7,
                            Message::Type::Command>
{
    quint32 k1 = {0};
    qint32  k2 = {0};
    lst::List<Point>  list;
    qint32 k3 = {0};

    DECLARE_J_SERIALIZE_FUNC
};
template <typename Packer>
Packer& TestCommand7::jserialize(Packer& p)
{
    p.startObject();
    p.member("k1") & k1;
    p.member("k2") & k2;
    p.member("list") & list;
    p.member("k3") & k3;
    return p.endObject();
}

struct TestCommand8 : Data<&command::TestCommand8,
                            Message::Type::Command>
{
    quint32 m1 = {0};
    lst::List<Point3>  list;
    quint64 m2 = {0};

    DECLARE_J_SERIALIZE_FUNC
};
template <typename Packer>
Packer& TestCommand8::jserialize(Packer& p)
{
    p.startObject();
    p.member("m1") & m1;
    p.member("m2") & m2;
    p.member("list") & list;
    return p.endObject();
}

} // namespace data
} // namespace communication


int main(int argc, char *argv[])
{
    using namespace communication;

    communication::data::TestCommand2 command2;
    command2.a = 5;
    command2.b = 7;
    QByteArray ba2 = command2.toJson();

    communication::data::TestCommand2 command22;
    command22.fromJson(ba2);

    communication::data::TestCommand3 command3;
    command3.command2 = command2;
    QByteArray ba3 = command3.toJson();

    communication::data::TestCommand3 command33;
    command33.fromJson(ba3);

    communication::data::TestCommand4 command4;
    command4.v << 1;
    command4.v << 2;
    command4.v << 3;
    QByteArray ba4 = command4.toJson();

    communication::data::TestCommand4 command44;
    command44.fromJson(ba4);

    communication::data::TestCommand5 command5;
    command5.f = 15;
    command2.a = 2;
    command2.b = 3;
    command5.v << command2;
    command2.a = 5;
    command2.b = 6;
    command5.v << command2;
    QByteArray ba5 = command5.toJson();

    communication::data::TestCommand5 command55;
    command55.fromJson(ba5);

    communication::data::TestCommand6 command6;
    command6.e2 = 85;
    command6.e3 = 90;
    command6.point = data::Point::Ptr(new data::Point);
    command6.point->x = 11;
    command6.point->y = 12;
    QByteArray ba6 = command6.toJson();

    communication::data::TestCommand6 command66;
    command66.fromJson(ba6);

    communication::data::TestCommand7 command7;
    command7.k1 = 53;
    command7.k2 = 18;
    command7.k3 = 19;
    data::Point* p = command7.list.add();
    p->x = 2;
    p->y = 3;
    p = command7.list.add();
    p->x = 8;
    p->y = 9;
    p = command7.list.add();
    p->x = 12;
    p->y = 16;
    p = command7.list.add();
    p->x = 20;
    p->y = 21;
    QByteArray ba7 = command7.toJson();

    communication::data::TestCommand7 command77;
    command77.fromJson(ba7);

    communication::data::TestCommand8 command8;
    command8.m1 = 101;
    command8.m2 = 200;
    data::Point3* p3 = command8.list.add();
    p3->x = 5;
    p3->y = 7;
    p3->z = 8;
    p3 = command8.list.add();
    p3->x = 11;
    p3->y = 15;
    p3->z = 19;
    p3 = command8.list.add();
    p3->x = 21;
    p3->y = 22;
    p3->z = 23;
    QByteArray ba8 = command8.toJson();

    communication::data::TestCommand8 command88;
    command88.fromJson(ba8);

    p3 = command88.list.item(2);

    communication::data::Unknown unknown;
    unknown.commandId = QUuid::createUuid();
    unknown.address = QHostAddress("127.0.0.5");
    unknown.port = 50051;
    unknown.socketName = "AAA";
    QByteArray baU = unknown.toJson();

    communication::data::Unknown unknownU;
    unknownU.fromJson(baU);

    communication::data::Unknown unknown2;
    unknown2.commandId = QUuid::createUuid();
    unknown2.address = QHostAddress("fe80::225:22ff:fec2:2afd");
    unknown2.port = 50051;
    unknown2.socketName = "AAA";
    QByteArray ba2U = unknown2.toJson();

    communication::data::Unknown unknown2U;
    unknown2U.fromJson(ba2U);


    return 0;
}

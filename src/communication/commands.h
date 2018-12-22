/*****************************************************************************
  В модуле представлен список идентификаторов команд для коммуникации между
  клиентской и серверной частями приложения.

  Требование надежности коммуникаций: однажды назначенный идентификатор коман-
  ды не должен более меняться.

*****************************************************************************/

#pragma once

#include "shared/list.h"
#include "shared/clife_base.h"
#include "shared/clife_ptr.h"
#include "shared/qt/quuidex.h"
#include "shared/qt/communication/serialization/json.h"
#include "shared/qt/communication/commands_base.h"
#include "shared/container_ptr.h"
#include <sys/time.h>

namespace communication {
namespace command {

//----------------------------- Список команд --------------------------------

// Команда на передачу видео-фреймов на удаленный терминал
extern const QUuidEx TestCommand1;
extern const QUuidEx TestCommand2;
extern const QUuidEx TestCommand3;

} // namespace command

//---------------- Структуры данных используемые в сообщениях ----------------

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

struct Point
{
    Point() = default;
    Point(quint64 x, quint64 y) : x(x), y(y) {}

    quint64 x = {0};
    quint64 y = {0};

    template <typename Packer>
    Packer& jserialize(Packer& p)
    {
        p.startObject();
        p.member("x") & x;
        p.member("y") & y;
        return p.endObject();
    }
};

struct TestCommand3 : Data<&command::TestCommand3,
                            Message::Type::Command>
{
    qint32 m = {0};
    qint64 k = {0};
    QVector<Point> points;

    DECLARE_J_SERIALIZE_FUNC
};
template <typename Packer>
Packer& TestCommand3::jserialize(Packer& p)
{
    p.startObject();
    p.member("m") & m;
    p.member("k") & k;
    p.member("points") & points;
    return p.endObject();
}

} // namespace data
} // namespace communication



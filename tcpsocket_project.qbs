import qbs
import qbs
import "tcpsocket_base.qbs" as TcpSocketBase

TcpSocketBase {
    name: "TcpSocket Servers"

    references: [
        "src/client2/client2.qbs",
        "src/rapidjson/rapidjson.qbs",
        "src/server1/server1.qbs",
        "src/server2/server2.qbs",
        "src/shared/shared.qbs",
        "src/yaml/yaml.qbs",
        "tests/jserialize_utest.qbs",
    ]
}

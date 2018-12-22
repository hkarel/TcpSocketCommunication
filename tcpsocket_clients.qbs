import qbs
import qbs
import "tcpsocket_base.qbs" as TcpSocketBase

TcpSocketBase {
    name: "TcpSocket Clients"

    references: [
        "src/client2/client2.qbs",
        "src/rapidjson/rapidjson.qbs",
        "src/shared/shared.qbs",
        "src/yaml/yaml.qbs",
    ]
}

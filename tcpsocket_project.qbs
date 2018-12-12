import qbs
import qbs
import "tcpsocket_base.qbs" as TcpSocketBase

TcpSocketBase {
    name: "TcpSocket Communication"

    references: [
        "src/server1/server1.qbs",
        "src/shared/shared.qbs",
        "src/yaml/yaml.qbs",
    ]
}

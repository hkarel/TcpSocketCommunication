import qbs
import QbsUtl
import ProbExt

Product {
    name: "Server1"
    targetName: "server1"
    condition: true

    type: "application"
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
    Depends { name: "SharedLib" }
    Depends { name: "Yaml" }
    Depends { name: "Qt"; submodules: ["core", "network"] }


    cpp.defines: {
        var def = project.cppDefines;
        return def;
    }

    cpp.cxxFlags: project.cxxFlags


    cpp.includePaths: [
        "./",
        "../",
    ]
    cpp.systemIncludePaths: QbsUtl.concatPaths(
        Qt.core.cpp.includePaths
    )

    cpp.dynamicLibraries: {
        var libs = [
            "pthread",
        ];
        return libs;
    }

     files: [
        "server1.cpp",
        "server1_appl.cpp",
        "server1_appl.h",
    ]

//    property var test: {
//        console.info("=== cpp.staticLibraries ===");
//        console.info(cpp.staticLibraries);
//        console.info("=== qbs.architecture ===");
//        console.info(qbs.architecture);
//        console.info("=== cpp.compilerLibraryPaths ===");
//        console.info(cpp.compilerLibraryPaths);
//        console.info("=== cpp.compilerPath ===");
//        console.info(cpp.compilerPath);
//        console.info("=== targetOS ===");
//        console.info(qbs.targetOS);
//        console.info("=== buildDirectory ===");
//        console.info(project.buildDirectory);
//    }

} // Product

import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.env import VirtualRunEnv
from conan.tools.files import copy, load

class QCryptoHistoryConan(ConanFile):
    name = "QCryptoHistory"
    license = "MIT"
    settings = ["os", "compiler", "build_type", "arch"]
    options = {
        "shared": [True, False],
        "build_testing": [True, False],
        "coverage_enable": [True, False],
        "enable_msvc_code_analysis": [True, False],
    }
    default_options = {
        "shared": True,
        "build_testing": False,
        "coverage_enable": False,
        "enable_msvc_code_analysis": False,
    }

    def layout(self):
        cmake_layout(self, src_folder=".", build_folder=".")

    def requirements(self):
        # self.requires("unqlitepp/0.0.1")
        self.requires("unqlitepp/0.0.1@unqlitepp/0.0.1")
        self.requires("qt/6.4.2")
        if self.options.build_testing:
            self.requires("catch2/3.3.2")

    def build_requirements(self):
        self.tool_requires("cmake/3.25.3")
        if self.settings.os == "Windows":
            self.tool_requires("ninja/1.11.1")
        else:
            self.tool_requires("make/4.3")
        # if self.options.build_testing:
        #     self.requires("catch2/3.3.2")

    def imports(self):
        # allow to run the binaries from Visual Studio without calling activate_run.bat
        # and can be useful to create an installer
        for dep in self.dependencies.values():
            for bindir in dep.cpp_info.bindirs:
                copy(self, "*.dll", bindir, os.path.join(self.build_folder, "bin"))

    def set_version(self):
        self.version = load(self, os.path.join(self.recipe_folder, "VERSION")).strip()

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTING"] = self.options.build_testing
        tc.variables["ENABLE_COVERAGE"] = self.options.coverage_enable
        tc.variables["ENABLE_MSVC_CODE_ANALYSIS"] = self.options.enable_msvc_code_analysis
        tc.generate()

        deps = CMakeDeps(self)
        # catch2 is used for both building (catch_discover_tests cmake macro) and running (unit test is linked to catch2main lib)
        deps.build_context_activated = ["catch2", "cmakecommonfiles"]
        deps.build_context_suffix = {"catch2": "_BUILD"}
        deps.build_context_build_modules = ["cmakecommonfiles"]
        deps.generate()

        runenv = VirtualRunEnv(self)
        runenv.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

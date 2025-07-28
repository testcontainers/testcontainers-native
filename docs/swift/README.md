# Using Testcontainers in Swift

The Testcontainers Native library is compatible with native languages, and that includes Swift. To use the library in Swift, a few extra steps must be followed in order for it to be useable.

## Using the generic Testcontainer Native API

From the root of the repository:

```bash
cmake .
cmake --build .
```

The testcontainers-native header and library must be deployed to /usr manually:

```bash
sudo cp testcontainers-c/testcontainers-native.h /usr/include
sudo cp testcontainers-c/testcontainers-native.so /usr/lib/libtestcontainers-native.so
```

This is required so that Swift can actually find the header and library. On some systems, it may be required to deploy the *.so file to `/usr/lib64` instead (such as openSUSE). Try this if Swift is unable to find the library at `/usr/lib`.

With a version of Swift 5.x installed, create a new project:

```bash
mkdir generic-container-swift && cd generic-container-swift
swift package init --type=executable
```

Inside of this project, create a new directory under `Sources/` called `CTestContainers`. Create a `CTestContainers.h` file with the following content:

```c
#include <testcontainers-native.h>
```

Then, create a file called `module.modulemap` in the same directory with this:

```c
module CTestContainers [system] {
    header "CTestContainers.h"
    link "testcontainers-native"
    export *
}
```

Move `main.swift` into a subdirectory in `Sources/` that matches the name of the project. You should end up with the following directory layout:

```bash
.
├── Package.swift
├── Sources
│   ├── CTestContainers
│   │   ├── CTestContainers.h
│   │   └── module.modulemap
│   └── generic-container-swift
│       └── main.swift
```

Then, add a the following definitions to the `targets` in `Package.swift`:

```swift
.systemLibrary(name: "CTestContainers")
```

It can be then included as a dependency for a target, like this:

```swift
.executableTarget(
    name: "generic-container-swift",
    dependencies: [
        "CTestContainers"
    ]
),
```

This module can then be imported and used from inside the application. However, the only challenge is dealing with C-style strings that are typically utf8 and are null terminated, so they require a bit of extra support for translation in Swift.

Here is the `generic-containers` demo translated into Swift, with an exstension to properly translate a `String` to a utf8 string for C:

```swift
import CTestContainers
import Glibc
import Foundation

extension String {
    func toCString() -> UnsafePointer<Int8> {
        return NSString(string: self).utf8String!
    }
}

let defaultImage = "wiremock/wiremock:3.0.1-1"
print("Using WireMock with the Testcontainers Native binding (from Swift):")

print("Creating new container: \(defaultImage)")
let requestId = tc_new_container_request(defaultImage)
tc_with_exposed_tcp_port(requestId, 8080)
tc_with_wait_for_http(requestId, 8080, "/__admin/mappings".toCString())
tc_with_file(requestId, "test_data/hello.json".toCString(), "/home/wiremock/mappings/hello.json".toCString())
let ret = tc_run_container(requestId)
defer {
    free(ret.r2)
}
let containerId = ret.r0
if ret.r1 == 0 {
    print("Failed to run the container: \(String(cString: ret.r2))")
    exit(-1)
}

print("Sending HTTP request to the container")
let response = tc_send_http_get(containerId, 8080, "/hello")
defer {
    free(response.r1)
    free(response.r2)
}
if response.r0 == -1 {
    print("Failed to send HTTP request: \(String(cString: response.r2))")
    exit(-1)
}
if response.r0 != 200 {
    print("Received wrong response code: \(response.r0) instead of 200")
    print(String(cString: response.r1!))
    print(String(cString: response.r2!))
    exit(-1)
}
print("Server Response: HTTP-\(response.r0)")
print(String(cString: response.r1!))
```

Also, note that that `free` should be called to avoid memory leaks on the C strings returned from the library. This is missing from the existing C demos, but is an important consideration.

## Using the WireMock Module

To use the `wiremock` module in Swift, all that needs to be done is to deploy the header and library in the same way as described above, and then add the following to `CTestContainers.h`:

```c
#include <testcontainers-native-wiremock.h>
```

And the following to `module.modulemap`:

```c
link "testcontainers-native-wiremock"
```

Then it is possible to use all the wiremock module helper functions from inside of Swift.

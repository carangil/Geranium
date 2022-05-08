This folder is where .h files for each Zcore subproject are copied after they are build.
Applications that use zcore (as well as test apps, or parts of zcore that depend on each other) should use this folder as the include directory.
.h files here should not be put in source control
BUT this file is here to both explain this, and '.gitkeep' this folder on a clean tree
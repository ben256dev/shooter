# Clone the repository
git clone https://github.com/libsdl-org/SDL.git tools/SDL3

function build_unix()
{
	mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=Release ..
	cmake --build . --config Release --parallel
	sudo cmake --install . --config Release
	cd ../
}

# Do the build steps (depends on what operating system you are using)
# Currently only macos and linux are supported (windows support coming soon)
cd tools/SDL3/
case "$(uname)" in
	Darwin) build_unix;;
	Linux) build_unix;;
	*)
		echo Currently unsupported os
		exit -1;;
esac
cd ../../

# Cleanup after ourselves
rm -rf tools/SDL3/

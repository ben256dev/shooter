# Clone the repository
git clone https://github.com/libsdl-org/SDL.git tools/SDL3

function build_linux()
{
	# Build SDL3
	cmake -S . -B build && cmake --build build --parallel $(nproc) && sudo cmake --install build
}

function build_macos()
{
	cmake -S . -B build && cmake --build build && sudo cmake --install build
}

# Do the build steps (depends on what operating system you are using)
# Currently only macos and linux are supported (windows support coming soon)
cd tools/SDL3/
case "$(uname)" in
	Darwin) build_macos;;
	*) build_linux;;
esac

cd ../../

# Cleanup after ourselves
rm -rf tools/SDL3/


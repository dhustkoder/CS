#!/bin/bash

BASEDIR=$(dirname $0)
SRCDIR="${BASEDIR}/src"
BUILDDIR="${BASEDIR}/build"



compileCC ()
{
	echo "BUILDING C AND C++ SOURCE CODE"
	C_OUTPUT_DIR="${BUILDDIR}/c"
	CXX_OUTPUT_DIR="${BUILDDIR}/cpp"
	C_SRC_DIR="${SRCDIR}/c"
	CXX_SRC_DIR="${SRCDIR}/cpp"
	CFLAGS="-std=c11 -Wall -Wextra -pedantic"
	CFLAGS_DEBUG="-O0 -ggdb -fsanitize=address -DCSDEBUG"
	CFLAGS_RELEASE="-O3"
	CXXFLAGS="-std=c++11 -Wall -Wextra -pedantic"
	CXXFLAGS_DEBUG=$CFLAGS_DEBUG
	CXXFLAGS_RELEASE=$CFLAGS_RELEASE

	if [ "$CC" == "" ]; then
		CC=gcc
	fi

	if [ "$CXX" == "" ]; then
		CXX=g++
	fi

	mode="${1}"

	if [[ $mode == "" || $mode == "debug" ]]; then
		mode="debug"
		CFLAGS="${CFLAGS} ${CFLAGS_DEBUG}"
		CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_DEBUG}"
	elif [[ $mode == "release" ]]; then
		CFLAGS="${CFLAGS} ${CFLAGS_RELEASE}"
		CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_RELEASE}"
	else
		echo "compileCC: Unknown mode \'${mode}\'"
		return
	fi

	echo "Building in ${mode} mode"

	FILETYPES=("*.c" "*.cpp")
	OUTDIRS=("$C_OUTPUT_DIR" "$CXX_OUTPUT_DIR")
	SRCDIRS=("$C_SRC_DIR" "$CXX_SRC_DIR")
	COMPILERS=("$CC" "$CXX")
	FLAGS=("$CFLAGS" "$CXXFLAGS")

	for i in 0 1; do

		if [ ! -d ${SRCDIRS[$i]} ]; then
			continue
		fi

		for file in $(find ${SRCDIRS[$i]} -name ${FILETYPES[$i]}); do
			fname=$(basename $file)

			if [ ! -d ${OUTDIRS[$i]} ]; then
				mkdir -p ${OUTDIRS[$i]}
			fi

			outname="${OUTDIRS[$i]}/${fname%${FILETYPES[$i]}}-${mode}.out"
			echo "${file}: ${outname}"
			${COMPILERS[$i]} ${FLAGS[$i]} -I ${SRCDIRS[$i]} -o $outname $file
		done
	done
}


compilePy ()
{
	echo "BUILDING PYTHON SOURCE CODE"
	wd=$(pwd)
	for pysrc in $(find "${SRCDIR}/python" -name '*test.py'); do
		cd $(dirname $pysrc)
		echo "${pysrc}: ${pysrc}"
		python -m py_compile $(basename ${pysrc})
		cd $wd
	done
}


compileJava ()
{
	echo "BUILDING JAVA SOURCE CODE"
	javac -Werror $(find "${SRCDIR}/java" -name '*.java')
}


compileCSharp ()
{
	echo "BUILDING C# SOURCE CODE"
	mode="$1"

	if [[ "$mode" == "release" ]]; then
		CSFLAGS="-optimize:+"
	elif [[ "$mode" == "" || "$mode" == "debug" ]]; then
		mode="debug"
		CSFLAGS="-define:CSDEBUG -optimize:-"
	fi

	echo "Building in ${mode} mode"

	CSINCLUDES=("${SRCDIR}/csharp/Common.cs" "${SRCDIR}/csharp/algorithms/sorting/ISortingAlgorithm.cs")

	for file in $(find "${SRCDIR}/csharp" -name '*.cs'); do

		shouldCompile=true

		for elem in ${CSINCLUDES[@]}; do
			if [[ "$elem" == "$file" ]]; then
				shouldCompile=false
				break
			fi
		done

		if [ $shouldCompile == true ]; then
			echo "Compiling ${file}..."
			mcs ${CSFLAGS} ${file} ${CSINCLUDES[@]}
		fi
	done
}


compileCC $@
compilePy $@
compileJava $@
compileCSharp $@


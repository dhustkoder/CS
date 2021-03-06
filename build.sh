#!/bin/bash

BASEDIR=$(dirname $0)
SRCDIR="${BASEDIR}/src"
BUILDDIR="${BASEDIR}/build"



compileCC ()
{
	echo "BUILDING C AND C++ SOURCE CODE"

	C_SRC_DIR="${SRCDIR}/c"
	C_OUTPUT_DIR="${BUILDDIR}/c"
	CFLAGS="-std=c11 -Wall -Wextra -pedantic -DDS_VECTOR -DDS_TYPE_INT"
	CFLAGS_DEBUG="-O1 -ggdb -fsanitize=address -DCSDEBUG"
	CFLAGS_RELEASE="-Ofast -fomit-frame-pointer -fno-math-errno -s -fno-ident       \
	                -ffunction-sections -fdata-sections -Wl,--gc-sections           \
			-fno-unwind-tables -fno-asynchronous-unwind-tables              \
			-fno-stack-protector -fomit-frame-pointer -fmerge-all-constants \
			-DNDEBUG"


	CXX_OUTPUT_DIR="${BUILDDIR}/cpp"
	CXX_SRC_DIR="${SRCDIR}/cpp"
	CXXFLAGS="-std=c++11 -Wall -Wextra -pedantic"
	CXXFLAGS_DEBUG=$CFLAGS_DEBUG
	CXXFLAGS_RELEASE="$CFLAGS_RELEASE -fno-rtti"


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
			echo "Compiling ${file}: ${outname}"
			${COMPILERS[$i]} ${FLAGS[$i]} -I ${SRCDIRS[$i]} -o $outname $file
		done
	done
}


compileCSharp ()
{
	echo "BUILDING C# SOURCE CODE"

	CSFLAGS="-warn:4 -warnaserror:+ -define:DS_TYPE_INT -define:DS_VECTOR"
	CSFLAGS_DEBUG="-define:CSDEBUG -optimize:-"
	CSFLAGS_RELEASE="-optimize:+"

	mode="$1"

	if [[ "$mode" == "release" ]]; then
		CSFLAGS="${CSFLAGS} ${CSFLAGS_RELEASE}"
	elif [[ "$mode" == "" || "$mode" == "debug" ]]; then
		mode="debug"
		CSFLAGS="${CSFLAGS} ${CSFLAGS_DEBUG}"
	else
		echo "Unkown mode ${mode}"
		return
	fi

	echo "Building in ${mode} mode"


	CSINCLUDES=("${SRCDIR}/csharp/Common.cs"
		    "${SRCDIR}/csharp/DataStructures/IDataStructure.cs"
		    "${SRCDIR}/csharp/DataStructures/Vector.cs")
	NOTCOMPILING=($CSINCLUDES
	              "${SRCDIR}/csharp/Algorithms/Searching/ISearchingAlgorithm.cs"
		      "${SRCDIR}/csharp/Algorithms/Sorting/ISortingAlgorithm.cs")


	CSFILES=$(find "${SRCDIR}/csharp/Algorithms/" -name '*.cs')

	mkdir -p "${BUILDDIR}/csharp"

	for file in ${CSFILES[@]}; do

		shouldCompile=true

		for elem in ${NOTCOMPILING[@]}; do
			if [[ "$elem" == "$file" ]]; then
				shouldCompile=false
				break
			fi
		done

		if [ $shouldCompile == false ]; then
			continue
		fi


		mainClass=${file#"${SRCDIR}/csharp/"}
		mainClass=${mainClass%'.cs'}

		index=0

		while [[ "$index" -lt ${#mainClass} ]]; do
			if [[ "${mainClass:$index:1}" == "/" ]]; then
				af=$((index + 1))
				mainClass="${mainClass:0:$index}.${mainClass:$af}"
			fi
			index=$((index + 1))
		done

		otherFiles=""
		for f in ${CSFILES}; do
			if [[ "$file" != "$f" ]]; then
				otherFiles="${otherFiles} ${f}"
			fi
		done

		outname=$(basename ${file})
		outname="${BUILDDIR}/csharp/${outname%'.cs'}-$mode.exe"
		echo "Compiling ${file}: ${outname}"
		mcs ${CSFLAGS} -main:${mainClass} ${file} ${otherFiles} ${CSINCLUDES[@]} -out:${outname}
	done
}


compileJava ()
{
	echo "BUILDING JAVA SOURCE CODE"
	javac -Xlint $(find "${SRCDIR}/java" -name '*.java')
}


compilePy ()
{
	echo "BUILDING PYTHON SOURCE CODE"
	wd=$(pwd)
	for pysrc in $(find "${SRCDIR}/python" -name '*test.py'); do
		cd $(dirname $pysrc)
		python -m py_compile $(basename ${pysrc})
		cd $wd
	done
}





compileCC $@
compileCSharp $@
compileJava $@
compilePy $@


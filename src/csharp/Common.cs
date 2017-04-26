using Algorithms.Sorting;
using Algorithms.Searching;
using System;
using DataStructures;


namespace Common
{
	public class Utils
	{
		public static Vector<int> MakeIntVectorFromStrings(string[] strs)
		{
			var vec = new Vector<int>();
			vec.Reserve(strs.Length);

			foreach (var str in strs)
				vec.PushBack(int.Parse(str));

			return vec;
		}


		public static void PrintData<T>(IDataStructure<T> data)
		{
			for (int i = 0; i < data.Count; ++i)
				Console.WriteLine("[" + i + "] = " + data[i]);
		}
	}

	
	public class Tests
	{
		public static int Sort(string[] args, ISortingAlgorithm sort)
		{
			var data = Utils.MakeIntVectorFromStrings(args);

			if (data.Count < 2) {
				Console.Error.WriteLine("Usage: " + AppDomain.CurrentDomain.FriendlyName + " [list]");
				return -1;
			}

			#if CSDEBUG
			Console.WriteLine("UNSORTED:");
			Utils.PrintData(data);
			#endif

			sort.Execute(data, (int a, int b) => { return a < b; });

			#if CSDEBUG
			Console.WriteLine("SORTED:");
			Utils.PrintData(data);
			#endif

			return 0;
		}


		public static int Search(string[] args, ISearchingAlgorithm search)
		{
			var data = Utils.MakeIntVectorFromStrings(args);

			if (data.Count < 3) {
				Console.Error.WriteLine("Usage: " + AppDomain.CurrentDomain.FriendlyName + " [list] [target]");
				return -1;
			}	

			int target = data.PopBack();

			#if CSDEBUG
			Console.WriteLine("ARRAY:");
			Utils.PrintData(data);
			#endif

			int idx = search.Execute(data, target);

			if (idx != data.Count)
				Console.WriteLine(target + " FOUND AT INDEX " + idx);
			else
				Console.WriteLine(target + " NOT FOUND");

			return 0;
		}

	}


}


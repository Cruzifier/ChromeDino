#ifndef UTILS_HPP
#define UTILS_HPP

class Utils {
	public:
		template<class Interface>
		/// <summary>
		/// Function to release directx interfaces
		/// </summary>
		/// <param name="interface_to_release">Interface to release</param>
		static void safeRelease(Interface **interface_to_release) {
			if (*interface_to_release != nullptr) {
				(*interface_to_release)->Release();
				(*interface_to_release) = nullptr;
			}
		}
};

#endif //UTILS_HPP
#pragma once

//-----------------------------------------------------------------------------
// Serialize methods for basic types.
// Cereal library is used for serialization.
//-----------------------------------------------------------------------------

#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

namespace cereal
{
	template <typename Archive, typename T>
	void serialize(
		Archive& archive,
		glm::tvec2<T>& data)
	{
		archive(
			cereal::make_nvp("x", data.x),
			cereal::make_nvp("y", data.y));
	};

	template <typename Archive, typename T>
	void serialize(
		Archive& archive,
		glm::tvec3<T>& data)
	{
		archive(
			cereal::make_nvp("x", data.x),
			cereal::make_nvp("y", data.y),
			cereal::make_nvp("z", data.z));
	};

	template <typename Archive, typename T>
	void serialize(
		Archive& archive,
		glm::tvec4<T>& data)
	{
		archive(
			cereal::make_nvp("x", data.x),
			cereal::make_nvp("y", data.y),
			cereal::make_nvp("z", data.z),
			cereal::make_nvp("w", data.w));
	};

	template <typename Archive, typename T>
	void serialize(
		Archive& archive,
		glm::tquat<T>& data)
	{
		archive(
			cereal::make_nvp("x", data.x),
			cereal::make_nvp("y", data.y),
			cereal::make_nvp("z", data.z),
			cereal::make_nvp("w", data.w));
	};

	template <typename Archive, typename T>
	void serialize(
		Archive& archive,
		glm::tmat2x2<T>& data)
	{
		for (glm::length_t i = 0; i < 2; ++i)
		{
			for (glm::length_t j = 0; j < 2; ++j)
			{
				archive(cereal::make_nvp(std::to_string(i * 2 + j), data[i][j]));
			}
		}
	};

	template <typename Archive, typename T>
	void serialize(
		Archive& archive,
		glm::tmat3x3<T>& data)
	{
		for (glm::length_t i = 0; i < 3; ++i)
		{
			for (glm::length_t j = 0; j < 3; ++j)
			{
				archive(cereal::make_nvp(std::to_string(i * 3 + j), data[i][j]));
			}
		}
	};

	template <typename Archive, typename T>
	void serialize(
		Archive& archive,
		glm::tmat4x4<T>& data)
	{
		for (glm::length_t i = 0; i < 4; ++i)
		{
			for (glm::length_t j = 0; j < 4; ++j)
			{
				archive(cereal::make_nvp(std::to_string(i * 4 + j), data[i][j]));
			}
		}
	};
};

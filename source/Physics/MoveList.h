#pragma once

class MoveList
{
public:
	struct Entry
	{
		Rigidbody rb;
		Vector2 pos;

		bool operator==(const MoveList::Entry& other) const
		{
			return (rb.entity == other.rb.entity);
		}
	};

	struct ColliderRanges
	{
		std::vector<MoveList::Entry>::iterator shipBegin;
		std::vector<MoveList::Entry>::iterator shipEnd;
		std::vector<MoveList::Entry>::iterator bulletBegin;
		std::vector<MoveList::Entry>::iterator bulletEnd;
		std::vector<MoveList::Entry>::iterator largeBegin;
		std::vector<MoveList::Entry>::iterator largeEnd;
		std::vector<MoveList::Entry>::iterator mediumBegin;
		std::vector<MoveList::Entry>::iterator mediumEnd;
		std::vector<MoveList::Entry>::iterator smallBegin;
		std::vector<MoveList::Entry>::iterator smallEnd;
	};

	void Enqueue(const MoveList::Entry& entry)
	{
		data.push_back(entry);
		++colliderCounts[(int)entry.rb.colliderType];
	}

	void Clear()
	{
		data.clear();
		for (auto& count : colliderCounts)
			count = 0;
	}

	size_t Size()
	{
		return data.size();
	}

	std::vector<MoveList::Entry>::iterator begin()
	{
		return data.begin();
	}
	std::vector<MoveList::Entry>::iterator end()
	{
		return data.end();
	}
	int GetColliderCount(const ColliderType& colliderType)
	{
		return colliderCounts[(int)colliderType];
	}

private:
	std::vector<MoveList::Entry> data;
	std::array<int, (int)ColliderType::COUNT> colliderCounts;
};

namespace std {
	template<> struct hash<MoveList::Entry>
	{
		inline size_t operator()(const MoveList::Entry& entry) const
		{
			return entry.rb.entity.hash();
		}
	};
}
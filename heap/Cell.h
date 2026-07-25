#pragma once


namespace Nyx {
    class Cell {
    public:
        enum class State {
            Dead,
            Alive,
        };

        bool is_marked() const { return m_marked; }
        bool is_alive() const { return m_state == State::Alive; }
        bool is_dead() const { return m_state == State::Dead; }

        void mark() { m_marked = true; }
        void unmark() { m_marked = false; }
        void set_alive() { m_state = State::Alive; }
        void set_dead() { m_state = State::Dead; }

    private:
        bool m_marked = false;
        State m_state = State::Dead;
    };
} // namespace Nyx

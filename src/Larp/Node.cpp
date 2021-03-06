#include "Node.hpp"

namespace Larp
{
    Node::Node()
        : _scale(glm::vec3(1.0f, 1.0f, 1.0f))
    {}

    void Node::draw(glm::mat4 model, glm::mat4& view, glm::mat4& projection, const glm::vec3& view_pos)
    {
        glm::mat4 tmp_model = glm::translate(glm::mat4(), this->_position);
        glm::mat4 rotation = glm::toMat4(this->_rotation);
        tmp_model = tmp_model * rotation;
        tmp_model = glm::scale(tmp_model, this->_scale);
        glm::mat4 my_model = model * tmp_model;

        if (this->_entity != nullptr)
            this->_entity->draw(my_model, view, projection, view_pos);
        for (auto& it : this->_children)
            it.second->draw(my_model, view, projection, view_pos);
    }

    void Node::draw_shadows(const glm::mat4& model)
    {
        glm::mat4 tmp_model = glm::translate(glm::mat4(), this->_position);
        glm::mat4 rotation = glm::toMat4(this->_rotation);
        tmp_model = tmp_model * rotation;
        tmp_model = glm::scale(tmp_model, this->_scale);
        glm::mat4 my_model = model * tmp_model;

        if (this->_entity != nullptr)
            this->_entity->draw_shadows(my_model);
        for (auto& it : this->_children)
            it.second->draw_shadows(my_model);
    }

    Node* Node::create_child()
    {
        Node* tmp = new Node();
        tmp->_parent = this;
        this->_children[tmp] = UniqueNode(tmp);
        return tmp;
    }

    Node* Node::remove_child(Node* child)
    {
        Node* ret_val = child;
        auto it = this->_children.find(ret_val);
        if (it == this->_children.end())
        {
            PRINT_ERROR("Child is not a child of this Node. Removing nothing.");
            return child;
        }
        it->second.release();       // Make sure we don't delete the child we are removing
        ret_val->_parent = nullptr; // Make the parent nullptr for testing when the user (possibly)
                                    // reatteches the Node somewhere else
        this->_children.erase(it);
        return ret_val;
    }

    void Node::delete_child(Node* child)
    {
        auto it = this->_children.find(const_cast<Node*>(child));
        if (it == this->_children.end())
        {
            PRINT_ERROR("Child is not a child of this Node. Removing nothing.");
        }
        this->_children.erase(it);
    }

    void Node::attach_child(Node* child)
    {
        if (child->_parent != nullptr)
        {
            THROW_RUNTIME_ERROR("This child is already attached to another Node");
        }

        Node* tmp = child;
        this->_children[tmp] = UniqueNode(tmp);
        tmp->_parent = this;
    }

    void Node::detach_this_from_parent()
    {
        if (this->_parent == nullptr)
        {
            PRINT_ERROR("This child is not attached to a Node. Ignoring.");
            return;
        }

        this->_parent->remove_child(this);
    }

    void Node::delete_this_from_scene()
    {
        if (this->_parent == nullptr)
        {
            PRINT_ERROR("This child is not attached to a Node. Ignoring.");
            return;
        }

        this->_parent->delete_child(this);
    }

    void Node::set_position(GLfloat x, GLfloat y, GLfloat z)
    {
        this->_position.x = x;
        this->_position.y = y;
        this->_position.z = z;
    }

    void Node::set_position(glm::vec3 position)
    {
        this->_position = position;
    }

    glm::vec3 Node::get_position()
    {
        return this->_position;
    }

    void Node::set_orientation(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        this->_rotation = glm::angleAxis(w, glm::vec3(x, y, z));
    }

    void Node::set_orientation(glm::quat rotation)
    {
        this->_rotation = rotation;
    }

    void Node::set_orientation(glm::vec3 axis, GLfloat w)
    {
        this->_rotation = glm::angleAxis(w, axis);
    }

    void Node::yaw(GLfloat yaw)
    {
        this->_rotation = glm::rotate(this->_rotation,
                                      yaw,
                                      glm::vec3(0, 1, 0));
    }

    void Node::pitch(GLfloat pitch)
    {
        this->_rotation = glm::rotate(this->_rotation,
                                      pitch,
                                      glm::vec3(1, 0, 0));
    }

    void Node::roll(GLfloat roll)
    {
        this->_rotation = glm::rotate(this->_rotation,
                                      roll,
                                      glm::vec3(0, 0, 1));
    }

    void Node::translate(GLfloat x, GLfloat y, GLfloat z)
    {
        this->_position += glm::vec3(x, y, z);
    }

    void Node::translate(glm::vec3 vec)
    {
        this->_position += vec;
    }

    void Node::set_scale(glm::vec3 scale)
    {
        this->_scale = scale;
    }

    void Node::set_scale(GLfloat x, GLfloat y, GLfloat z)
    {
        this->_scale.x = x;
        this->_scale.y = y;
        this->_scale.z = z;
    }

    glm::vec3 Node::get_scale() const
    {
        return this->_scale;
    }

    void Node::attach_entity(Entity* entity)
    {
        this->_entity.reset(entity);
    }

    void Node::remove_entity()
    {
        this->_entity.reset(nullptr);
    }

    Entity* Node::detach_entity()
    {
        return this->_entity.release();
    }

    GLfloat Node::get_scaled_width() const
    {
        return this->_entity->get_width() * this->_scale.x;
    }

    GLfloat Node::get_scaled_height() const
    {
        return this->_entity->get_height() * this->_scale.y;
    }

    GLfloat Node::get_scaled_depth() const
    {
        return this->_entity->get_depth() * this->_scale.z;
    }

    glm::vec3 Node::get_scaled_bounding_box() const
    {
        return glm::vec3(this->_entity->get_width() * this->_scale.x,
                         this->_entity->get_height() * this->_scale.y,
                         this->_entity->get_depth() * this->_scale.z);
    }
}

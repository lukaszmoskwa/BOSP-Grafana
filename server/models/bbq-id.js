module.exports = (sequelize, type) => {
    return sequelize.define('bbq_id', {
        id: {
            type: type.INTEGER,
            primaryKey: true,
            autoIncrement: true
        },
        sys: {
            type: type.INTEGER,
            allowNull: false,
        },
        bbq_group: {
            type: type.INTEGER,
            allowNull: false,
        },
        unit_type: {
            type: type.INTEGER,
            allowNull: false,
        },
        unit_id: {
            type: type.INTEGER,
            allowNull: false,
        },
        type_string: {
            type: type.STRING,
            allowNull: false,
        },
        pe_type: {
            type: type.INTEGER,
            allowNull: false,
        },
        pe_id: {
            type: type.INTEGER,
            allowNull: false,
        },
    }, {
        underscored: true
    })
}
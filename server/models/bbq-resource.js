module.exports = (sequelize, type) => {
    return sequelize.define('bbq_resource', {
        id: {
            type: type.INTEGER,
            primaryKey: true,
            autoIncrement: true
        },
        model: {
            type: type.STRING,
            allowNull: false,
        },
        occupancy: {
            type: type.INTEGER,
            allowNull: false,
        },
        bbq_load: {
            type: type.INTEGER,
            allowNull: false,
        },
        power: {
            type: type.INTEGER,
            allowNull: false,
        },
        temp: {
            type: type.INTEGER,
            allowNull: false,
        },
        fans: {
            type: type.INTEGER,
            allowNull: false,
        },
    }, {
        underscored: true,
        updatedAt: false
    })
}
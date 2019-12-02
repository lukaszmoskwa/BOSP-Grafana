module.exports = (sequelize, type) => {
  return sequelize.define(
    'bbq_app',
    {
      id: {
        type: type.INTEGER,
        primaryKey: true,
        autoIncrement: true,
      },
      pid: {
        type: type.INTEGER,
        allowNull: false,
      },
      name: {
        type: type.STRING,
        allowNull: false,
      },
      state: {
        type: type.STRING,
        allowNull: false,
      },
    },
    {
      underscored: true,
    },
  );
};

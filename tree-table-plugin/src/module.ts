import { MetricsPanelCtrl } from 'grafana/app/plugins/sdk';
import defaultsDeep from 'lodash/defaultsDeep';

import { DataFrame } from '@grafana/data';

interface KeyValue {
  key: string;
  value: any;
}

interface BBQData {
  columns?: Array<{ text: string }>;
  refId?: string | undefined;
  type?: string;
  rows?: [];
}

interface BBQRes {
  res_name: string;
  occupancy: number;
}

interface BBQRow {
  id: number;
  app_name: string;
  app_state: string;
  resources: BBQRes[];
}

interface PanelDefaultInterface {
  text: string;
  bbq_rows: BBQRow[];
  bbq_cols: [];
}

export default class SimpleCtrl extends MetricsPanelCtrl {
  static templateUrl = 'partials/module.html';

  panelDefaults: PanelDefaultInterface = {
    text: 'Hello Worldss',
    bbq_rows: [],
    bbq_cols: [],
  };

  // Simple example showing the last value of all data
  firstValues: KeyValue[] = [];

  /** @ngInject */
  constructor($scope, $injector) {
    super($scope, $injector);
    defaultsDeep(this.panel, this.panelDefaults);

    // Get results directly as DataFrames
    (this as any).dataFormat = 'series';

    this.events.on('init-edit-mode', this.onInitEditMode.bind(this));
    this.events.on('render', this.onRender.bind(this));
    this.events.on('data-received', this.handleDataFrame.bind(this));
    this.events.on('data-error', this.onDataError.bind(this));
  }

  onInitEditMode() {
    this.addEditorTab('Options', `public/plugins/${this.pluginId}/partials/options.html`, 2);
  }

  onRender() {
    if (!this.panel.bbq_final.length) {
      return;
    }
    console.log('render');

    // Tells the screen capture system that you finished
    this.renderingCompleted();
  }

  getResourceType(res: number) {
    switch (res) {
      case 3:
        return 'CPU';
      case 4:
        return 'GPU';
      case 5:
        return 'ACC';
      case 7:
        return 'MEM';
      case 8:
        return 'NET';
      default:
        return 'none';
    }
  }

  onDataError(err: any) {
    console.log('onDataError', err);
  }

  // 6.3+ get typed DataFrame directly
  handleDataFrame(data: DataFrame[]) {
    const [bbqAppData, bbqIDData]: BBQData[] = data;
    console.log(data);
    // Check for all data existance
    if (bbqAppData && bbqIDData && bbqAppData.rows && bbqIDData.rows && bbqAppData.columns) {
      this.panel.bbq_cols = [];
      for (const col of bbqIDData.rows) {
        this.panel.bbq_cols.push({
          bbq_id: col[0],
          type_string: col[1],
          unit_id: col[2],
          pe_id: col[3],
        });
      }
      this.panel.bbq_rows = [];
      for (const row of bbqAppData.rows) {
        const temp = {};
        for (const [i, col] of bbqAppData.columns.entries()) {
          temp[col.text] = row[i];
        }
        this.panel.bbq_rows.push(temp);
      }
      const fin = {};
      for (const val of this.panel.bbq_rows) {
        if (
          fin[val.app_pid] &&
          fin[val.app_pid].resources.length < this.panel.bbq_cols.length &&
          fin[val.app_pid].resources.filter(el => el.bbq_id === val.bbq_id).length === 0
        ) {
          fin[val.app_pid].resources.push({
            bbq_id: val.bbq_id,
            occupancy: val.occupancy,
          });
        } else if (!fin[val.app_pid]) {
          fin[val.app_pid] = {
            app_name: val.app_name,
            app_state: val.app_state,
            resources: [
              {
                bbq_id: val.bbq_id,
                occupancy: val.occupancy,
              },
            ],
          };
        }
      }
      this.panel.bbq_final = fin;
      this.panel.bbq_app_array = Object.keys(fin);
    } else {
      return;
    }
    console.log(this.panel);
  }
}
export { SimpleCtrl as PanelCtrl };
